#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

//valor padrão do intervalo de medição em segundos
float interval = 15.0;
int unit = 1;

extern const char PROGMEM index_html[]; //String HTML
extern const char PROGMEM get_html[];
extern const char PROGMEM css[];
extern const char PROGMEM script_interval[];

AsyncWebServer server(80);
DNSServer dns;
//Gateway padrão: 192.168.4.1

//Retorna erro 404 se o client não puder se comunicar com o server
void notFound(AsyncWebServerRequest *request){
  request->send(404, "text/html", "Not Found");
}

void initAsyncWebServer() {

  //E necessário inicializar rede WiFi -> ou seja, deve vir depois de WiFi.begin()
  
/*Mas o que é isso? Fleivio?
  Muita calma nessa hora meu consagrado
  Isso é uma magnífica expressão lambda
  O parâmetro "on" para o objeto server indica que a função será executada sempre que for chamada, dispensando a necessidade de um loop
  O "/" indica a rota traçada pela requisição, assim, quando fordes acessar o arquivo coloque ip/ no navegador
  O segundo parãmetro, HTTP_GET, define o método do request
  O terceiro e ultimo parametro é uma função anonima lambda.
  os itens entre [] são as variáveis a serem capturadas, não nos importam agora
  os itens entre () são os parametros de entrada, no caso o AsyncWebServerRequest *request é o nosso client
  "request->send_P" envia os dados de "index_html" para o client
  */

  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ //envia o a string html para o client
    request->send_P(200, "text/html", index_html); //utilizamos send_P porque os dados estão na memeria flash (PROGMEM)
    });
 
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){ //quando o client submitar, nosso action o joga na rota de "/get"
    if(request->hasParam("input1") && request->hasParam("unit")){
      
      interval = request->getParam("input1")->value().toFloat(); //busca o valor float do input intervalo
      unit = request->getParam("unit")->value().toInt(); //busca o valor int do input unit

      interval = interval * unit; //coloca o intervalo na unidade imformada

      request->send(200, "text/html", get_html);
    }
    else{
      request->send(200, "text/html", "<p>Parâmetro não encontrado</p> <br> <a href=\"/\">Tentar novamente</a>"); //Essa parte me encomoda bastante, mas já é um começo
    }
  });

   server.on("/interval", HTTP_GET, [](AsyncWebServerRequest *request){ //quando o javascript fizer uma requisição em /interval
    request->send(200, "text/plain", String(interval)); //enviamos o text/plain com o intervalo
    });

    server.on("/css.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/css", css);
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/javascript", script_interval);
      });
    
    /*server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", temperature); 
    });

    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", humidity); 
    });*/


  server.onNotFound(notFound); //chama a função que retorna Erro 404
  
  server.begin();
}
