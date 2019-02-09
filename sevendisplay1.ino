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

// A, B, C, D, E, F, G, DP
const unsigned char NUMBERS[] = {
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

void showDigit(int digitIdx, unsigned char number)
{
  const int firstControlPin = 10;
  digitalWrite(firstControlPin + digitIdx, HIGH);
  const int segmentFirstPin = 1;
  unsigned char mask = B10000000;
  for (int i = 0; i < 8; ++i) {
    digitalWrite(segmentFirstPin + i, (NUMBERS[number] & mask) ? LOW : HIGH);
    mask >>= 1;
  }
  delay(5);
  digitalWrite(firstControlPin + digitIdx, LOW);
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
