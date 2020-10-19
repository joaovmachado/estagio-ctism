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
  float vout = ( value * ( 5.0 / 1024.0 ) );
  float resLDR = ( 10.0 * ( 5 - vout ) ) / vout;
  float lux = 500.0/resLDR;

  return lux;
}
