unsigned long interval = 15000;

int getInterval() {

  Serial.println("\nIniciando LittleFS...");
  if (!LittleFS.begin()) {
    Serial.println(" [ERRO]");
    return 15000;
  }
  
  interval = readFile("/data/interval.txt").toInt();
  LittleFS.end();
  //Serial.print("Intervalo definido para: "); Serial.println(interval);
  return interval;
}

void setInterval( int value ) {
  Serial.println("\nIniciando LittleFS...");
  if (!LittleFS.begin()) {
    Serial.println(" [ERRO]");
    Serial.println("\t O valor de interval não foi alterado");
    return;
  }

  writeFile("/data/interval.txt", (String)value); //Escreve o intervalo obtido na memoria
}


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
  //Ver server.serveStatic -> SPIFFS
  server.on("/", HTTP_GET, [](){ //envia o a string html para o client
    server.send_P(200, "text/html", index_html); //utilizamos send_P porque os dados estão na memeria flash (PROGMEM)
  });
 
  server.on("/get", HTTP_GET, [](){ //quando o client submitar, nosso action o joga na rota de "/get"
    if (server.hasArg("input1")){

      String response = server.arg("input1");
      
      int index = response.indexOf(":");

      int hours = response.substring(0, index).toInt() * 60; // Extrai o valor das horas e converte para minutos
      int minu = response.substring(index + 1).toInt() + hours; // Extrai o valor dos minutos e soma às horas

      interval = minu * 60 * 1000; // Converte para milissegundos

      setInterval(interval);
      
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
