#include "project_header.h"

//Configurações de Sensores
DHT dht(DHT_PIN, DHT_TYPE);

//Configurações da Interface de Rede Wifi
//const char *ssid = "";//Identificador de rede   <-- MODIFICAR DE ACORDO COM A REDE
//const char *passwd = "";//Senha                 <-- ʕ•ᴥ•ʔ

// Informações básicas do servidor...
const char *host =  "www.megatecnologia.com.br"; //URL servidor
const char *route = "/controle/silas.json";
const char *query = "?chave=523DA-0D1DD-A84D9-EF34B-F1B31-99AC9-28"; //Chave da aplicação |parâmetro chave|

//Parâmetros de Configuração |SETUP| disponíveis via página de configuração do servidor
extern const char PROGMEM index_html[]; //String HTML
extern ESP8266WebServer server;
extern DNSServer dns;

//Inicia o contador que receberá o valor de millis a cada envio
//Seu overflow deve ocorrer a cada 50 dias, aproximadamente
unsigned long counter = 0;
unsigned long timerControl;
unsigned int interval_timestamp = 0;
extern unsigned long interval;

byte error_status;
bool no_error = true;
int blink_counter = 0;
//Todos essa valores desconsideram um possível atraso na execução do programa
// Arquivos de armazenamento de dados
const char * sensors_data_path = "/data/data.csv";
const char * interval_file_path = "/data/interval.txt";
 
/** 
 *  Configuração do cliente NTP e do timedate input
 **/

char custom_time[64];
char custom_date[64];

unsigned long int custom_unixTimestamp;

#define DEBUG_NTPClient // Ativa o debug da lib NTPClient.h
WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, -3 * 3600);
  
void setup()
{
  pinMode(POWER_LED, OUTPUT);
  digitalWrite(POWER_LED, HIGH); //Liga led sinalizador, indicando que o programa foi inicializado
  
  Serial.println("\nIniciando LittleFS...");
     if (!LittleFS.begin()) {
     Serial.println(" [ERRO]");
  }

  strcpy(custom_time, readFile("/time/custom-time.txt").c_str()); // Parâmetros customizados no Portal de Configuração
  strcpy(custom_date, readFile("/time/custom-date.txt").c_str());
  
  Serial.begin(115200);

    deleteFile(interval_file_path);
    deleteFile(sensors_data_path);
    deleteFile("/time/custom-data.txt");
    deleteFile("/time/custom-time.txt");
    
  setLedsPinMode(); //Inicializa pinMode dos leds de sinalização como output
  initWiFiManager();
  initWebServer();
  displayNetInfo(); // Exibe SSID, IP e RSSI da rede na comunicacao Serial
  saveSTAIp(); // Guarda o IP obtido no modo Station na memória 

  //ntpClient.begin();
  
  dht.begin();
  interval = getInterval();

  ntpClient.forceUpdate();
}

void loop()
{
  server.handleClient();

  if ( (timerControl = millis()) - counter >= interval ) {
    no_error = true; //reseta variável
    turn_off_leds(); //apaga todos os LEDs de sinalização
    //        requestServer();  REQUISÇÃO AO SERVIDOR DESATIVADA
    //        Serial.println("Enviando Requisição ao servidor");

                                                                      // CASO NÃO HAJA INTERNERT, NÂO HAVERÁ ACESSO AO SERVIDOR NTP
    if (!ntpClient.isTimeSet()) {  // Se o tempo não for definido automaticamente pelo servidor NTP
      Serial.println("O tempo não foi definido.");

      ntpClient.setCurrentEpoc(custom_unixTimestamp);
      Serial.println("Tempo customizado: ");
      Serial.println(ntpClient.getFormattedTime());
      Serial.println("Data customizada: ");
      Serial.println(getFormattedDate());
    }
    
    appendFile(sensors_data_path, (String)dht.readTemperature() + "," + (String)dht.readHumidity() + "," + ntpClient.getFormattedTime() + " " + getFormattedDate() + "\n");
    readFile(sensors_data_path);
    
    Serial.print("Intervalo definido para: "); Serial.println(interval);
    
    counter = timerControl;

    if (no_error) error_status = 0;
  }

  if ( (millis() - interval_timestamp) >= 1000 ) {
    //Serial.println("blink = " + (String)blink_counter);
    switch (error_status) {
      case 255:
        //pisca azul a cada segundo
        led_waiting();
        break;
      case 0:
        //pisca verde uma vez
        led_success();
        if (++blink_counter == 2) {
          error_status = 255;
          blink_counter = 0;
        }
        break;
      case 1: //connection failed (blink 2x)
        Serial.println("connection failed");
        led_error();
        if (++blink_counter == 4) {
          error_status = 255;
          blink_counter = 0;
        }
        break;
      case 2: //not retorned 2xx (blink 2x)
      Serial.println("not retorned 2xx");
       led_error();
        if (++blink_counter == 4) {
          error_status = 255;
          blink_counter = 0;
        }
       break;
     case 3: //memory read error (blink 3x)
       Serial.println("memory read error");
       led_error();
       if (++blink_counter == 6) {
          error_status = 255;
          blink_counter = 0;
       }
       break;
    }
    interval_timestamp = millis();
  }
}


String getFormattedDate (void) {
  time_t epochTime = ntpClient.getEpochTime();
  Serial.println("=========NTPClient.getEpochTime() in getFormattedDate()==========");
  Serial.println(ntpClient.getEpochTime());
  Serial.println(epochTime);
  struct tm *ptm = gmtime(&epochTime); 
 
  unsigned long monthDay = ptm->tm_mday;
  String monthDayStr = (monthDay < 10) ? "0" + String(monthDay) : String(monthDay);
 
  unsigned long currentMonth = ptm->tm_mon + 1;
  String currentMonthStr = (currentMonth < 10) ? "0" + String(currentMonth) : String(currentMonth);
 
  unsigned long currentYear = ptm->tm_year + 1900;
  String currentYearStr = String(currentYear);

  Serial.println("=========TM Structure inside getFormattedDate()==========");
  Serial.println(ptm->tm_mday);
  Serial.println(ptm->tm_mon);
  Serial.println(ptm->tm_year); //ERRO

  Serial.println("=========cY, cM, cmD in getFormattedDate()==========");
  Serial.println(currentYearStr);
  Serial.println(currentMonthStr);
  Serial.println(monthDayStr);

  
  //Print complete date:
  String currentDate = monthDayStr + "/" + currentMonthStr + "/" + currentYearStr;
  return currentDate;
}


unsigned long convertToUnixTimestamp (String date_str, String time_str)
{
  uint8_t mday = (date_str.substring(0, 2)).toInt();
  uint8_t month = (date_str.substring(3, 5)).toInt();
  uint16_t year = (date_str.substring(6)).toInt();

  uint8_t hour = (time_str.substring(0, 2)).toInt();
  uint8_t minute = (time_str.substring(3, 5)).toInt();
  uint8_t sec = (time_str.substring(6)).toInt();

  
  Serial.println("==========Datatime Parsed=========");
  Serial.println(year);
  Serial.println(month);
  Serial.println(mday);
  Serial.println(hour);
  Serial.println(minute);
  Serial.println(sec);


  struct tm ct{};
  ct.tm_year = year - 1900;
  ct.tm_mon = month - 1;
  ct.tm_mday = mday;
  ct.tm_hour = hour + 3; //Acrescenta-se +3 para compensar o UTC-3 informado pelo usuário
  ct.tm_min = minute;
  ct.tm_sec = sec;

  Serial.println("==========TM STRUCTURE=========");
  Serial.println(ct.tm_year);
  Serial.println(ct.tm_mon);
  Serial.println(ct.tm_mday);
  Serial.println(ct.tm_hour);
  Serial.println(ct.tm_min);
  Serial.println(ct.tm_sec);

  unsigned long currentEpoc = (unsigned long)mktime(&ct);
  Serial.println("==========CUSTOM_TIMESTSMP in UNIX EPOCH UTC-3=========");
  Serial.println(currentEpoc);

  return currentEpoc;
}

float convertToLux ( int value )
{
  float vin = 3.3;
  float vout = ( value * ( vin / 1023 ) ); // tensão
  float resLDR = ( 10.0 * ( vin - vout ) ) / vout;
  float lux = 500.0/resLDR;

  return lux;
}
