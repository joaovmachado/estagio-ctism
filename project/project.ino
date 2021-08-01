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
const char * last_ntp_timestamp_path = "/time/timestamp.txt"; // NAO UTILIZADO

char custom_time[65]; // Parâmetros customizados no Portal de Configuração
char custom_date[65];

WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, "pool.ntp.org",  -3 * 3600);
  
void setup()
{
  pinMode(POWER_LED, OUTPUT);
  digitalWrite(POWER_LED, HIGH); //Liga led sinalizador, indicando que o programa foi iniciado
  Serial.println("\nIniciando LittleFS...");
     if (!LittleFS.begin()) {
     Serial.println(" [ERRO]");
  }
  Serial.begin(115200);  
  setLedsPinMode(); //Inicializa pinMode dos leds de sinalização como output
  initWiFiManager();
  initWebServer();
  displayNetworkConfiguration(); //Exibe SSID, IP e RSSI da rede na comunicacao Serial
  saveAPIP();

  ntpClient.begin();
  
  dht.begin();
  interval = getInterval();

  ntpClient.forceUpdate();
}

void loop()
{
  server.handleClient();

  if ( (timerControl = millis()) - counter >= interval ) {
    no_error = true; //reseta variável
    turn_off_leds();
    //        requestServer();  REQUISÇÃO AO SERVIDOR DESATIVADA
    //        Serial.println("Enviando Requisição ao servidor");

                                                                      // CASO NÃO HAJA INTERNERT, NÂO HAVERÁ ACESSO AO SERVIDOR NTP
    if (!ntpClient.isTimeSet()) {  // Se o tempo não for definido
      Serial.println("O tempo não foi definido");
      Serial.print("Tempo definido salvo na memória: "); // O tempo salvo na memória deverá ser usado
      Serial.println(custom_time);
      Serial.print("Data definida na memória: ");
      Serial.println(custom_date);
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
  unsigned long epochTime = ntpClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime); 
 
  unsigned long monthDay = ptm->tm_mday;
  String monthDayStr = (monthDay < 10) ? "0" + String(monthDay) : String(monthDay);
 
  unsigned long currentMonth = ptm->tm_mon + 1;
  String currentMonthStr = (currentMonth < 10) ? "0" + String(currentMonth) : String(currentMonth);
 
  unsigned long currentYear = ptm->tm_year + 1900;
  
  //Print complete date:
  String currentDate = monthDayStr + "/" + currentMonthStr + "/" + String(currentYear);
  return currentDate;
}
