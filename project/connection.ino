// Função que gerencia configuração da rede com WiFiManager
void initWiFiManager()
{
  //WiFi.begin(ssid, passwd);
  //WiFi.config(ip, gateway, subnet);

  WiFiManager wm;

  // Exibe o STA IP salvo na memória no topo da página de configuração, útil para acessar o webserver no modo STA, ainda que impreciso
  String lastIPSaved = "<p style=\"text-align=center\">Último IP registado: " + readFile("/data/ipaddress.txt") + "</p>";
  wm.setCustomHeadElement(lastIPSaved.c_str());

  // Configura os parâmetros customizados para serem exibidos no formulário de configuração da rede WiFi, a saber, hora e data corrente
  // id/name, placeholder/prompt, default, length {WiFiManagerParameter Sintax}
  WiFiManagerParameter custom_time_parameter("customTimeParameter", "Hora atual (Formato: HH:MM:SS) ", (strcmp(custom_time, "failed") != 0) ? custom_time : "", 8);
  WiFiManagerParameter custom_date_parameter("customDateParameter", "Data atual (Formato: dd/mm/aaaa) ", (strcmp(custom_date, "failed") != 0) ? custom_date : "", 10);
  wm.addParameter(&custom_time_parameter);
  wm.addParameter(&custom_date_parameter);
  
  //wm.setCustomHeadElement("<p style=\"text-align=center\"><a href='/'>Baixar dados dos sensores</a></p>");

  //      wm.setConfigPortalTimeout(30);
  //wm.startConfigPortal("ESP8266", "redeesp8266");  // Inicia a página de configuração, sem consultar a memória
  wm.autoConnect("AP_ESP"); // Fuça pelas últimas credenciais salvas na memória - Seria interessante setar essa opcao no portal de configuracao
  
  ntpClient.resetLastUpdate(); // A contagem da sincronização deve ser inicializada somente após a configuração no Config Portal

  // Copia os valores obtidos para custom_time e custom_date 
  strcpy(custom_time, custom_time_parameter.getValue());
  strcpy(custom_date, custom_date_parameter.getValue());

  writeFile("/time/custom-time.txt", custom_time);
  writeFile("/time/custom-date.txt", custom_date);

  // A data informada é convertida em segundos desde 01/01/1970 (Unix Epoch)
  custom_unixTimestamp = convertToUnixTimestamp(custom_date, custom_time);
}


// Função que retorna os dados de configuração da rede para a Serial, exibe a interface de rede: SSID, IP e RSSI da conexão
void displayNetInfo() //Network Interface Status
{
  Serial.println("\n--------NetInfo--------");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}


void requestServer()
// Envia uma requisição para o servidor pré-definido
{
  WiFiClient client;

  Serial.print("Gerando pacote HTTP... ");
  String requestPkg = createRequest();
  Serial.println("[OK]");

  Serial.printf("\nConectando-se a %s... ", readFile("/host.txt").c_str());
  if ( client.connect( readFile("/host.txt"), 80 )) {
    Serial.println("[OK]");
    Serial.print("Enviando Requisição... ");
    client.print(requestPkg.c_str());
    Serial.println("[OK]");
    Serial.println("\n[Request:]");
    Serial.println(requestPkg);
    appendFile("/etc/request.log", "\n\n\n[Request:]\n" + requestPkg);
  }
  else {
    error_status = 1; no_error = false;
    Serial.println("[ERRO]");
    Serial.println("Houve uma falha ao tentar estabelecer conexão com " + readFile("/host.txt"));
    appendFile("/etc/request.log", "Houve uma falha ao tentar estabelecer conexão com " + readFile("/host.txt"));
  }

  bool is200 = false;
  bool isfirst_line = true;

  Serial.println("\n[Response:]");
  appendFile("/etc/request.log", "\n\n[Response:]\n");
  
  while ( client.connected() || client.available() ) {
    if ( client.available() ) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
      
      appendFile("/etc/request.log", line);

      if (isfirst_line == true && line.indexOf("2", 9) != -1) {
        is200 = true;
      }

      isfirst_line = false;
    }
  }
  appendFile("/etc/request.log", "\n");
  Serial.print("is200? "); Serial.println(is200);
  Serial.print("no_error? "); Serial.println(no_error);
  
  if (!is200 && no_error) {
    error_status = 2; no_error = false;
  }

  if (!client.connected()) {
    Serial.print("Encerrando conexão... ");
    client.stop();
    Serial.println("[Disconnected]");
  }
}



String createRequest()
// Retorna uma String contendo uma requisição HTTP POST contendo um JSON no corpo do pacote
{
  //extern String json;
  //String body = json;

  String jsonParsed = readFile("/json.txt");
  jsonParsed.replace("[T]", (String)dht.readTemperature());
  jsonParsed.replace("[H]", (String)dht.readHumidity());
  jsonParsed.replace("[TM_DATE]", (String)getFormattedTimeDate());
  jsonParsed.replace("[L]", (String)analogValueToPercent(analogRead(LDR_PIN)));

  String body = jsonParsed;

  String req;
  req =  "POST " + readFile("/path.txt") + readFile("/query.txt") + " HTTP/1.1\r\n";
  req += "Host: " + readFile("/host.txt") + "\r\n";
  req += "Connection: close\r\n";
  req += "User-Agent: NodeMCU v3\r\n";
  req += "Content-Type: application/json\r\n";
  req += "Content-Length: " + (String)body.length() + "\r\n";
  req += "Accept: */*\r\n";

  req += "\r\n" + body;

  return req;
}
