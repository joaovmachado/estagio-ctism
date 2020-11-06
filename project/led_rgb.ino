#define RED_LED 15
#define GREEN_LED 13
#define BLUE_LED 12

void setLedsPinMode() {
  pinMode(15, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void led_error(int del, int rep) {
  for(int i = 0; i < rep; i++){
    digitalWrite(RED_LED, HIGH);
    delay(del);
    digitalWrite(RED_LED, LOW);
    delay(del);
  }
}

void led_connecting(int del) {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(del);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(del); 
}

void led_connection(int del, int rep) {
  for(int i = 0; i < rep; i++){
    digitalWrite(GREEN_LED, HIGH);
    delay(del);
    digitalWrite(GREEN_LED, LOW);
    delay(del);
  }
}

void led_sending_data() {
   digitalWrite(BLUE_LED, HIGH);
   delay(50);
   digitalWrite(BLUE_LED, LOW);
   delay(50);
}
