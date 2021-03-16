unsigned long interval = 15000;

int getInterval() {

  Serial.println("\nIniciando LittleFS...");
  if (!LittleFS.begin()) {
    Serial.println(" [ERRO]");
    return 15000;
  }
  
  interval = readFile("/data/interval.txt").toInt();
  LittleFS.end();
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

void initWebServer() {
  //E necessário inicializar rede WiFi -> ou seja, deve vir depois de WiFi.begin()
  //Ver server.serveStatic -> SPIFFS
  
  server.on("/", HTTP_GET, [](){
    server.send_P(200, "text/html", index_html); //utilizamos send_P porque os dados estão na memeria flash (PROGMEM)
  });
 
  server.on("/get", HTTP_GET, [](){ //quando o client submitar, nosso action o joga na rota de "/get"
    if (server.hasArg("input1")){

      if(server.hasArg("sendNow")){
        if(server.arg("sendNow") == "1"){
          counter = timerControl; //zera o contador de tempo decorrido
          Serial.println("\nEnviando dados imediatamente e zerando contador...");
          requestServer();
        }
      }

      String response = server.arg("input1");

      int hours = response.substring(0, 2).toInt() * 60; // Valor das horas -> min
      int minutes = (response.substring(3, 5).toInt() + hours) * 60; // Valor dos min -> sec
      int seconds = response.substring(6, 8).toInt() + minutes; // Valor dos seconds
      interval = seconds * 1000; // valor dos seconds -> milissegundos

      setInterval(interval);
      
      server.send_P(200, "text/html", get_html);
    }
    else {
      server.send(200, "text/html", "<p>Parâmetro não encontrado</p> <br> <a href=\"/\">Tentar novamente</a>"); //Essa parte me incomoda bastante, mas já é um começo
    }
  });

  server.on("/restart", HTTP_GET, [](){
    server.send(204);
    ESP.restart();
  });

// REQUISIÇÕES DO HTML
   server.on("/css.css", HTTP_GET, [](){
    server.send_P(200, "text/css", css);
  });

  server.on("/script.js", HTTP_GET, [](){
    server.send_P(200, "text/javascript", script_interval);
  });
//

// REQUISIÇÕES DO JAVASCRIPT
  server.on("/interval", HTTP_GET, [](){ 
    server.send(200, "text/plain", String(interval));
  });
   
  server.on("/temperature", HTTP_GET, [](){
    server.send(200, "text/plain", String(dht.readTemperature())); 
  });

  server.on("/humidity", HTTP_GET, [](){
    server.send(200, "text/plain", String(dht.readHumidity())); 
  });

  server.on("/luminosity", HTTP_GET, [](){
    server.send(200, "text/plain", String( convertToLux(analogRead(LDR_PIN)) )); 
  });
//

// Chama a função que retorna Erro 404
  server.onNotFound(notFound); 
  
  server.begin();
}
