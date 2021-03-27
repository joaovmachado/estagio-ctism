/*void readDHT()
{
    int temperature = dht.readTemperature();
    int humidity = dht.readHumidity();

    Serial.println("\n[Sensors:]");
    Serial.println("Temperatura: " + (String)temperature);
    Serial.println("Umidade: " + (String)humidity);
}
*/

float convertToLux ( int value )
{
  float vin = 3.3;
  float vout = ( value * ( vin / 1023 ) ); // tensão
  float resLDR = ( 10.0 * ( vin - vout ) ) / vout;
  float lux = 500.0/resLDR;

  return lux;
}
