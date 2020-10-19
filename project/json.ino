//JSON que será utilizado na criação de uma requisição em connection.ino
String json = "{\"Temperatura\": " + (String)dht.readTemperature() + ",\"Umidade\": " + (String)dht.readHumidity() + "Luminosidade: " + convertToLux(analogRead(LDR_PIN)) + ",\"SensorID\":\"1234\"}";
