/*
 *  AAA
 * F   B
 *  FFF
 * E   C
 *  DDD  DP
*/
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
const unsigned char LETTERS[] = {
  B11101110, //A
  B11111110, //B
  B10011100, //C
  B11111100, //D
  B10011110, //E
  B10001110, //F
  B10111110, //G
  B01101110, //H
  B01100000, //I
  B01110000, //J
  B01101110, //K
  B00011100, //L
  B11101100, //M
  B01101110, //N
  B11111100, //O
  B11001110, //P
  B11111101, //Q
  B11101110, //R
  B10110110, //S
  B11100000, //T
  B01111100, //U
  B01111100, //V
  B01111100, //W
  B01101110, //X
  B01100110, //Y
  B11011010, //Z
};
const unsigned char DOT = B00000001; //.
const unsigned char EXCLAMATION = B01100001; //!
const unsigned char QUESTION = B11001011; //?
const unsigned char SPACE = B0;
int controlFirstPin = 10;
int segmentFirstPin = 1;

void initSevenSegmentDisplay(int firstControlPin, int firstSegmentPin)
{
	controlFirstPin = firstControlPin;
	segmentFirstPin = firstSegmentPin;
}

void writeSymbol(int digitIdx, unsigned char symbolData)
{
  digitalWrite(controlFirstPin + digitIdx, HIGH);
  unsigned char mask = B10000000;
  for (int i = 0; i < 8; ++i) {
    digitalWrite(segmentFirstPin + i, (symbolData & mask) ? LOW : HIGH);
    mask >>= 1;
  }
  delay(3);
  digitalWrite(controlFirstPin + digitIdx, LOW);
}

void showDigit(int digitIdx, unsigned char number)
{
  writeSymbol(digitIdx, NUMBERS[number]);
}

unsigned char getSymbolData(char symbol)
{
  if (symbol >= '0' && symbol <= '9') 
  {
   return NUMBERS[symbol - '0'];
  }
  else if (symbol >= 'A' && symbol <= 'Z')
  {
    return LETTERS[symbol - 'A'];
  }
  else if (symbol >= 'a' && symbol <= 'z')
  {
     return LETTERS[symbol - 'a'];
  }
  else if (symbol == '.')
  {
    return DOT;
  }
  else if (symbol == '!')
  {
    return EXCLAMATION;
  }
  else if (symbol == '?')
  {
    return QUESTION;
  }
  return SPACE;
}

void showSymbol(int digitIdx, char symbol) 
{
  writeSymbol(digitIdx, getSymbolData(symbol));
}

void showNumber(unsigned int number)
{
  showDigit(0, number / 1000 % 10);
  showDigit(1, number / 100 % 10);
  showDigit(2, number / 10 % 10);
  showDigit(3, number % 10);
}

void showString(const char* str, int len, int offset)
{
  for (int i = 0; i < 4; ++i) {
    showSymbol(i, str[(offset + i) % len]);
  }
}
