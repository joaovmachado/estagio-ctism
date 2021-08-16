int getInterval() {

  String hold_interval_mstring = readFile(interval_file_path); //valor salvo na memória que define o intervalo

  //Tratamento de exceção caso o valor lido na memória retorne um erro
  if (hold_interval_mstring != "failed") {
    interval = hold_interval_mstring.toInt();
  }
  else if (!LittleFS.exists(interval_file_path)) {
      Serial.println("Não foi encontrado o arquivo /data/interval.txt, definindo intervalo para 2 minutos. [Forçado]");
      writeFile(interval_file_path, "120000");
      interval = 120000;
  }
  return interval;
}


void setInterval( int set_value ) {
  writeFile(interval_file_path, (String)set_value); //Escreve o intervalo obtido na memoria
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

  server.on("/request-config", HTTP_GET, [](){
    server.send_P(200, "text/html", request_config_html);

    if (server.hasArg("host")) {
      writeFile("/host.txt", server.arg("host"));
      
      if (server.hasArg("path")) {
        writeFile("/path.txt", server.arg("path"));
      }
      else {
        writeFile("/path.txt", "/");
      }

      if (server.hasArg("query")) {
        writeFile("/query.txt", "?" + server.arg("query"));
      }

      writeFile("/http-method.txt", server.arg("http-method"));
      writeFile("/content-type.txt", server.arg("content-type"));
      writeFile("/json.txt", server.arg("json")); // apesar de json, pode armazenar um xml como conteúdo, mantido json apenas por questão de versionamento
    }
  });

  server.on("/data", HTTP_GET, [](){

    File file = LittleFS.open(sensors_data_path, "r");
    server.streamFile(file, "text/csv");
    file.close();
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

      String response = server.arg("input1"); //input1 = input do intervalo

      int hours = response.substring(0, 2).toInt() * 60; // Valor das horas -> min
      int minutes = (response.substring(3, 5).toInt() + hours) * 60; // Valor dos min -> sec
      int seconds = response.substring(6, 8).toInt() + minutes; // Valor dos seconds
      interval = seconds * 1000; // valor dos seconds -> milissegundos, a leitura do intervalo é totalmente convertida em milissegundos

      setInterval(interval); // Função que armazena o intervalo na memória flash do esp8266, não confundir com a função setInterval definida no JavaScript da página
      
      ntpClient.setUpdateInterval(interval - 10000); // Atualiza o tempo de sincronização com o servidor para o novo intervalo definido

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

  server.on("/delete-sensors-data", HTTP_GET, [](){
    if (server.arg("trigger") == "true"){
      deleteFile(sensors_data_path);  
    }
    server.sendHeader("Location", "/", true); // Redireciona para a homepage
    server.send(302); 
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
    server.send(200, "text/plain", String( analogValueToPercent(analogRead(LDR_PIN)) ) + "%"); 
  });

  server.on("/request-log", HTTP_GET, [](){
    File file = LittleFS.open("/etc/request.log", "r");
    server.streamFile(file, "text/plain");
    file.close();
  });
//

// Chama a função que retorna Erro 404
  server.onNotFound(notFound); 
  
  server.begin();
}
