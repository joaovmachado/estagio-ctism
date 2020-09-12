#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <FS.h>
#include <DHT.h>

#define DHT_PIN       2//               <-- MODIFICAR DE ACORDO COM AS CONFIGURAÇÕES DOS SENSORES
#define DHT_TYPE      DHT11//          <--      ~(˘▾˘~)   ♥‿♥   (~˘▾˘)~
#define LDR_PIN       A0
#define SIGNAL_LED    D5
//#include <ESP6266WebServer.h>

//Protótipos de funções
//connection_functions
void initAsyncWebServer();
void ifStatus();
void requestServer();

//Sensors
float convertToLux ( int value );
//

//Configurações de Sensores
DHT dht(DHT_PIN, DHT_TYPE);

//Configurações da Interface de Rede Wifi
const char *ssid = "";//Identificador de rede   <-- MODIFICAR DE ACORDO COM A REDE
const char *passwd = "";//Senha                 <-- ʕ•ᴥ•ʔ

// Informações básicas do servidor...
const char *host =  "megatecnologia.com.br"; //URL servidor
const char *route = "/controle/silas.json";
const char *query = "?chave=523DA-0D1DD-A84D9-EF34B-F1B31-99AC9-28"; //Chave da aplicação |parâmetro chave|

//Parâmetros de Configuração |SETUP|
extern float interval;
extern const char PROGMEM index_html[]; //String HTML
extern AsyncWebServer server;
extern DNSServer dns;

//Inicia o contador que receberá 
//Seu overflow deve ocorrer a cada 50 dias, aproximadamente
unsigned long counter = 0;
//Todos essa valores desconsideram um possível atraso na execução do programa

void setup()
{
    Serial.begin(115200);

    //WiFi.begin(ssid, passwd);
    //WiFi.config(ip, gateway, subnet);

    
    AsyncWiFiManager wifiManager(&server,&dns);
    wifiManager.autoConnect("AP_ESP");
    initAsyncWebServer();
    ifStatus();
    
    //dht.begin();
}

void loop()
{
    unsigned long timerControl = millis();


  
    if ((unsigned long)timerControl - counter >= interval*1000) {
      Serial.println("[LDR SENSOR:]\n");
      Serial.println(convertToLux(analogRead(LDR_PIN)));
      //requestServer();
      counter = timerControl;
      Serial.println("[Enviando dados ao servidor]");
    }
}
