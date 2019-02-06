// the setup function runs once when you press reset or power the board
const int numberOfLeds = 3;
const int led1Id = 9;
const int stepTime = 2000;
const int fullCycle = stepTime * 3;
const int maxBrightness = 255;

void setup() {
  for (int i = 0; i < numberOfLeds; ++i) 
  {
    pinMode(led1Id + i, OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  const unsigned long currentMillis = millis() % fullCycle;
  const int firstLedIdx = currentMillis / stepTime;
  const int secondLedIdx = (firstLedIdx + 1) % 3;
  const long firstBrightness = (currentMillis % stepTime) * maxBrightness / stepTime;
  const long secondBrightness = maxBrightness - firstBrightness;
  for (int i = 0; i < numberOfLeds; ++i) 
  {
    if (i == firstLedIdx) 
    {
      analogWrite(led1Id + i, secondBrightness);
    } 
    else if (i == secondLedIdx)
    {
      analogWrite(led1Id + i, firstBrightness);
    }
    else
    {
      analogWrite(led1Id + i, LOW);
    }
  }
  delay(10);
}
