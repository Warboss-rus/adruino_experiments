// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(11, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(12, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(10, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(12, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(10, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(11, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);
}
