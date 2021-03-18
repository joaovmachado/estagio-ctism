#define RED_LED 15
#define GREEN_LED 13
#define BLUE_LED 12

void setLedsPinMode() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void led_error() {
  Serial.println("[Led_ERROR] " + (String)(digitalRead(RED_LED)) + " -> " + (String)(!digitalRead(RED_LED)));
  digitalWrite(RED_LED, (!digitalRead(RED_LED)));
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void led_waiting() {
  Serial.println("[Led_WAITING] " + (String)(digitalRead(RED_LED)) + " -> " + (String)(!digitalRead(BLUE_LED)));
  digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void led_success() {
   Serial.println("[Led_SUCCESS] " + (String)(digitalRead(RED_LED)) + " -> " + (String)(!digitalRead(GREEN_LED)));
   digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
   digitalWrite(RED_LED, LOW);
   digitalWrite(BLUE_LED, LOW);
}
