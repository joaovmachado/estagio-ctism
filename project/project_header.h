#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DHT.h>
#include <LittleFS.h>   //https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
#include <NTPClient.h>  //https://github.com/arduino-libraries/NTPClient

#define DEBUG_NTPClient // Ativa o debug da lib NTPClient.h
//#define TM_DEBUG      // Debug das funções de tempo e data

#define DHT_PIN       D7//             <-- MODIFICAR DE ACORDO COM AS CONFIGURAÇÕES DOS SENSORES
#define DHT_TYPE      DHT11//          <--      ~(˘▾˘~)   ♥‿♥   (~˘▾˘)~
#define LDR_PIN       A0

#define POWER_LED     D5
#define RED_LED       D1
#define GREEN_LED     D2
#define BLUE_LED      D6

  //Protótipos de funções
    //connection.ino
    void initWiFiManager( void );                       // Inicializa WiFiManager
    void displayNetInfo( void );                        // Retorna os dados de configuração da rede para a Serial, exibe a interface de rede: SSID, IP e RSSI da conexão
    void requestServer( void );                         // Envia uma requisição para o servidor pré-definido
    String createRequest( void );                       // Retorna uma String contendo uma requisição HTTP POST contendo um JSON no corpo do pacote

    //filesystem.ino  -  Funções de leitura e escrita no sistema de arquivos montado
    String readFile(const char * path);                 // Faz a leitura de um arquivo
    void writeFile(const char * path, String input);    // Grava uma String em um arquivo
    void appendFile(const char * path, String message); // Anexa uma String em um arquivo
    void saveSTAIp( void );                             // Grava o último IP definido no modo Station
    
    //led_rgb.ino  -  Funções de controle dos LEDs no circuito do projeto
    void setLedsPinMode( void );                        // Define os pinos digitais conectados aos LEDs como pinos de saída
    void led_portal_waiting( void );                    // Ativa o LED vermelho e azul ao mesmo tempo, deve ser usado para indicar espera de configuração
    void led_error( void );                             // Ativa LED vermelho
    void led_waiting( void );                           // Ativa LED azul
    void led_success( void );                           // Ativa LED verde
    void turn_off_leds( void );                         // Desliga todos os LEDs que estiverem ligados
    
    //sensors.ino  -  Funções relacionadas aos sensores
    float analogValueToPercent (int value);             // Retorna a medição em porcentagem de um sinal análógico como entrada

    //server.ino  -  Funções relacionadas com o manuseio de requisições e de respostas do webserver inicizalizado
    void initWebServer( void );                         // Inicializa Servidor interno do Esp8266
    int getInterval( void );                            // Retorna o valor de intervalo salvo na memória flash do esp8266
    void setInterval(int set_value);                    // Armazena um inteiro de entrada na memória flash do esp8266

    String getFormattedDate ( void );                   // Retorna a data recuperada do sevidor NTP após a última atualização
    String getTimeDate();                               // Retorna o tempo e a data no formto dd/mm/aaaa HH:MM:SS
    String getFormattedTimeDate ( void );               // Retorna o tempo e a data no formato iso-8601
