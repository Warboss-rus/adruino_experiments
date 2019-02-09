#include <sevendisplaylib.h>

void setup() {
  for (int i = 1; i <= 8; ++i) 
  {
    pinMode(i, OUTPUT);
  }
  for (int i = 10; i <= 13; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  showString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 'Z' - 'A' + 1, (millis() / 1000));
}
