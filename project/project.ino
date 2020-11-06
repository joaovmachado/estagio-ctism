#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DHT.h>
#include <FS.h>
#include <LittleFS.h>

#define DHT_PIN       2//               <-- MODIFICAR DE ACORDO COM AS CONFIGURAÇÕES DOS SENSORES
#define DHT_TYPE      DHT11//          <--      ~(˘▾˘~)   ♥‿♥   (~˘▾˘)~
#define LDR_PIN       A0
#define SIGNAL_LED    D5

  //Protótipos de funções
    void initAsyncWebServer();
    void ifStatus();
    void requestServer();
    float convertToLux ( int value );
  //

//Configurações de Sensores
DHT dht(DHT_PIN, DHT_TYPE);

//Configurações da Interface de Rede Wifi
extern unsigned long interval;
const char *ssid = "";//Identificador de rede   <-- MODIFICAR DE ACORDO COM A REDE
const char *passwd = "";//Senha                 <-- ʕ•ᴥ•ʔ

// Informações básicas do servidor...
const char *host =  "megatecnologia.com.br"; //URL servidor
const char *route = "/controle/silas.json";
const char *query = "?chave=523DA-0D1DD-A84D9-EF34B-F1B31-99AC9-28"; //Chave da aplicação |parâmetro chave|

//Parâmetros de Configuração |SETUP| disponíveis via página de configuração do servidor
extern const char PROGMEM index_html[]; //String HTML
extern ESP8266WebServer server;
extern DNSServer dns;

//Inicia o contador que receberá 
//Seu overflow deve ocorrer a cada 50 dias, aproximadamente
unsigned long counter = 0;
//Todos essa valores desconsideram um possível atraso na execução do programa

void setup()
{
  Serial.begin(115200);
  LittleFS.begin();
  setLedsPinMode(); //Inicializa pinMode dos leds de sinalização como output

  //WiFi.begin(ssid, passwd);
  //WiFi.config(ip, gateway, subnet);

  WiFiManager wifiManager;
  wifiManager.autoConnect("AP_ESP"); // Fuça pelas últimas credenciais salvas na memória
  // wifiManager.startConfigPortal(); // Inicia a página de configuração, sem consultar a memória
  initAsyncWebServer();
  ifStatus();
  //dht.begin();
  interval = getInterval();
}

void loop()
{
  server.handleClient();
  unsigned long timerControl = millis();

  if ( (unsigned long)timerControl - counter >= interval ) {
    //requestServer();
    Serial.println("Enviando Requisição ao servidor");
    Serial.print("Intervalo definido para: "); Serial.println(interval);
    counter = timerControl;
  }
}
