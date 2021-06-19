void setLedsPinMode() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void led_error() {
  //Serial.println("[Led_ERROR] " + (String)(digitalRead(RED_LED)) + " -> " + (String)(!digitalRead(RED_LED)));
  digitalWrite(RED_LED, (!digitalRead(RED_LED)));
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void led_waiting() {
  //Serial.println("[Led_WAITING] " + (String)(digitalRead(BLUE_LED)) + " -> " + (String)(!digitalRead(BLUE_LED)));
  digitalWrite(BLUE_LED, (!digitalRead(BLUE_LED)));
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void led_success() {
   //Serial.println("[Led_SUCCESS] " + (String)(digitalRead(GREEN_LED)) + " -> " + (String)(!digitalRead(GREEN_LED)));
   digitalWrite(GREEN_LED, (!digitalRead(GREEN_LED)));
   digitalWrite(RED_LED, LOW);
   digitalWrite(BLUE_LED, LOW);
}

void turn_off_leds() {
   Serial.println("[disabling all LEDs]");
   digitalWrite(GREEN_LED, LOW);
   digitalWrite(RED_LED, LOW);
   digitalWrite(BLUE_LED, LOW);
}
