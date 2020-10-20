//JSON que será utilizado na criação de uma requisição em connection.ino
//String json = "{\"Temperatura\": 24,\"Umidade\": 76,\"SensorID\":\"1234\"}";

String json = "{\"Temperatura\": " + (String)dht.readTemperature() + ",\"Umidade\": " + (String)dht.readHumidity() + ",\"SensorID\":\"1234\"}";
