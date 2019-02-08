void setup() {
  for (int i = 1; i <= 7; ++i) 
  {
    pinMode(i, OUTPUT);
  }
  for (int i = 10; i <= 13; ++i)
  {
    pinMode(i, OUTPUT);
  }
}

// A, B, C, D, E, F, G, DP
const unsigned int NUMBERS[] = {
  B11111100,
  B01100000,
  B11011010,
  B11110010,
  B01100110,
  B10110110,
  B10111110,
  B11100000,
  B11111110,
  B11110110,
};
const unsigned int MASKS[] = {
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001,
};

void showDigit(int digitIdx, unsigned char number)
{
  const int firstControlPin = 10;
  for (int i = 0; i <= 3; ++i) {
    digitalWrite(firstControlPin + i, i == digitIdx ? HIGH : LOW);
  }
  const int segmentFirstPin = 1;
  for (int i = 0; i < 7; ++i) {
    digitalWrite(segmentFirstPin + i, !(NUMBERS[number] & MASKS[i]) ? HIGH : LOW);
  }
  delay(5);
}

void showNumber(unsigned int number) 
{
  showDigit(0, number / 1000 % 10);
  showDigit(1, number / 100 % 10);
  showDigit(2, number / 10 % 10);
  showDigit(3, number % 10);
}

void loop() {
  // put your main code here, to run repeatedly:
  showNumber((millis() / 10) % 10000);
}
