#define RED_LED 15
#define GREEN_LED 13
#define BLUE_LED 12

void setLedsPinMode() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void led_error(int del, int rep) {
  Serial.print("[Led_ERROR] ");
  unsigned int delay_timestamp = 0;
  int i = 0;
  while (i < (rep * 2)) {
    if ( millis() - delay_timestamp >= del ) {
      digitalWrite(RED_LED, (!digitalRead(RED_LED)));
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      i++;
      Serial.print(i); Serial.print(", ");
      delay_timestamp = millis();
      yield();
    }
  }
  Serial.println();
}

void led_connecting(int del) {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(del);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(del); 
}

void led_waiting() {
  Serial.println("[Led_WAITING] ");
  digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

void led_sending_data() {
   Serial.println("[Led_SENDING_DATA] ");
   digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
   digitalWrite(RED_LED, LOW);
   digitalWrite(GREEN_LED, LOW);
}
