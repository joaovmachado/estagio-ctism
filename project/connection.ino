/*
  CONTEÚDO
    startWiFi (inicia conexão WiFi)
    initWiFiManager (gerencia configuração da rede com WiFiManager)
    displayNetworkConfiguration (exibe interface de rede: SSID, IP e RSSI da conexão)
    requestServer (prepara requisição ao servidor)
        createRequest (cria pacote de requisição)
*/

void startWiFi()
{
  if ( WiFi.status() == WL_NO_SHIELD ) {
    Serial.println("O Shield Wifi não foi reconhecido!");
    Serial.println("Programa Finalizado...");
    while (true); //Interrompe execução do programa
  }

  Serial.print("Tentando estabelecer comunicação com " + (String)ssid);
  while ( WiFi.status() != WL_CONNECTED ) {
    //Testa conexão com rede WPA/WPA2
    delay(500);
    Serial.print(".");
  }
  Serial.println("[OK]\n");
}

void initWiFiManager()
{
  //WiFi.begin(ssid, passwd);
  //WiFi.config(ip, gateway, subnet);

  WiFiManager wifiManager;

  String lastIPSaved = "<p style=\"text-align=center\">Último IP registado: " + readFile("/data/ipaddress.txt") + "</p>";
  wifiManager.setCustomHeadElement(lastIPSaved.c_str());

  if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(60);

  wifiManager.startConfigPortal();
  
  //wifiManager.autoConnect("AP_ESP"); // Fuça pelas últimas credenciais salvas na memória
  // wifiManager.startConfigPortal(); // Inicia a página de configuração, sem consultar a memória
}

void displayNetworkConfiguration() //Network Interface Status
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());

  long rssi = WiFi.RSSI();
  Serial.println("Intensidade do Sinal (RSSI): " + (String)rssi + " dBm.");
}

void requestServer()
{
  WiFiClient client;

  Serial.print("Gerando pacote HTTP... ");
  String requestPkg = createRequest();
  Serial.println("[OK]");

  Serial.printf("\nConectando-se a %s... ", host);
  if ( client.connect(host, 80) ) {
    Serial.println("[OK]");
    Serial.print("Enviando Requisição... ");

    led_sending_data(); //Liga led
    
    client.print(requestPkg);
    Serial.println("[OK]");
    Serial.println("\n[Request:]");
    Serial.println(requestPkg);

    led_sending_data(); //Desliga led
  }
  else {
    led_error(1000, 2);
    Serial.println("[ERRO]");
    Serial.println("Houve uma falha ao tentar estabelecer conexão com " + (String)host);
  }

  bool is200 = false; //ainda incerto (pode ser false)
  bool isfirst_line = true;
  Serial.println("\n[Response:]");
  while ( client.connected() || client.available() ) {
    if ( client.available() ) {
      String line = client.readStringUntil('\n');
      Serial.println(line);

      if (isfirst_line == true && line.indexOf("2", 9) != -1) {
        is200 = true;
        isfirst_line = false;
        Serial.print("is200? "); Serial.println(is200);
      }
    }
  }
  if (!is200) led_error(1000, 3);

  if ( !client.connected() ) {
    Serial.print("Encerrando conexão... ");
    client.stop();
    Serial.println("[Disconnected]");
  }
}

String createRequest()
{
  //extern String json;
  //String body = json;

  String body = "{\"Temperatura\": " + (String)dht.readTemperature() + ",\"Umidade\": " + (String)dht.readHumidity() + ",\"SensorID\":\"1234\"}";
  
  String req;
  req =  "POST " + (String)route + (String)query + " HTTP/1.1\r\n";
  req += "Host: " + (String)host + "\r\n";
  req += "User-Agent: NodeMCU v3\r\n";
  req += "Content-Type: application/json\r\n";
  req += "Content-Length: " + (String)body.length() + "\r\n";
  req += "Accept: */*\r\n";

  req += "\r\n" + body;

  return req;
}
