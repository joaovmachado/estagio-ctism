#include <ESP8266WebServer.h>
#include <FS.h>

//valor padrão do intervalo de medição em segundos
float interval = 15.0;
int unit = 1;

extern const char PROGMEM index_html[]; //String HTML
extern const char PROGMEM get_html[];
extern const char PROGMEM css[];
extern const char PROGMEM script_interval[];

ESP8266WebServer server(80);
DNSServer dns;
//Gateway padrão: 192.168.4.1

//Retorna erro 404 se o client não puder se comunicar com o server
void notFound() {
  server.send(404, "text/html", "Not Found");
}

void initAsyncWebServer() {

  //E necessário inicializar rede WiFi -> ou seja, deve vir depois de WiFi.begin()
  
  server.on("/", HTTP_GET, [](){ //envia o a string html para o client
    server.send_P(200, "text/html", index_html); //utilizamos send_P porque os dados estão na memeria flash (PROGMEM)
  });
 
  server.on("/get", HTTP_GET, [](){ //quando o client submitar, nosso action o joga na rota de "/get"
    if (server.hasArg("input1") && server.hasArg("unit")){
      
      interval = server.arg("input1").toFloat(); //busca o valor float do input intervalo
      unit = server.arg("unit").toInt(); //busca o valor int do input unit

      interval = interval * unit; //coloca o intervalo na unidade informada

      server.send(200, "text/html", get_html);
    }
    else {
      server.send(200, "text/html", "<p>Parâmetro não encontrado</p> <br> <a href=\"/\">Tentar novamente</a>"); //Essa parte me encomoda bastante, mas já é um começo
    }
  });

  //Requisições do HTML
   server.on("/css.css", HTTP_GET, [](){
    server.send_P(200, "text/css", css);
  });

  server.on("/script.js", HTTP_GET, [](){
    server.send_P(200, "text/javascript", script_interval);
  });

  //Requisições do Javascript
  server.on("/interval", HTTP_GET, [](){ 
    server.send(200, "text/plain", String(interval));
  });
   
  server.on("/temperature", HTTP_GET, [](){
    server.send(200, "text/plain", String(dht.readTemperature())); 
  });

  server.on("/humidity", HTTP_GET, [](){
    server.send(200, "text/plain", String(dht.readHumidity())); 
  });
  


  server.onNotFound(notFound); //chama a função que retorna Erro 404
  
  server.begin();
}
