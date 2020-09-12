/*
    startWiFi (inicia conexão WiFi)
    ifStatus (exibe interface de rede)
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

  Serial.print("Tentando estabelecer comunicação com " + (String)ssid + "\n");
  while ( WiFi.status() != WL_CONNECTED ) {
    //Testa conexão com rede WPA/WPA2
    delay(500);
    Serial.print(".");
  }
  Serial.println("[OK]\n");
}

void ifStatus() //Network Interface Status
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
    client.print(requestPkg);
    Serial.println("[OK]");
    Serial.println("\n[Request:]");
    Serial.println(requestPkg);
  }
  else {
    Serial.println("[ERRO]");
    Serial.println("Houve uma falha ao tentar estabelecer conexão com " + (String)host);
  }

  Serial.println("\n[Response:]");
  while ( client.connected() || client.available() ) {
    if ( client.available() ) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }

  if ( !client.connected() ) {
    Serial.print("Encerrando conexão... ");
    client.stop();
    Serial.println("[Disconnected]");
  }
}

String createRequest()
{
  String body = "{\"Temperatura\": " + (String)dht.readTemperature() + ",\"Umidade\": " + (String)dht.readHumidity() + "Luminosidade: " + convertToLux(analogRead(LDR_PIN)) + ",\"SensorID\":\"1234\"}";

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
