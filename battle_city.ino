#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TILE_ROWS 16
#define TILE_COLUMNS 8
byte TANK_SPRITE[] = {
  B00001000,
  B11001011,
  B01001010,
  B11111111,
  B01111110,
  B11111111,
  B01111110,
  B11011011,
};
byte BRICK_SPRITE[] = {
  B10111011,
  B10111011,
  B00000000,
  B11101110,
  B11101110,
  B00000000,
  B10111011,
  B10111011,
};
byte HQ_SPRITE[] = {
  B11011011,
  B01010010,
  B01111110,
  B00111100,
  B00111100,
  B00011000,
  B00111100,
  B01011010,
};
byte ARMOUR_SPRITE[] = {
  B11101110,
  B11101110,
  B11101110,
  B00000000,
  B11101110,
  B11101110,
  B11101110,
  B00000000,
};
enum TileType
{
  NONE = 0,
  BRICK = 1,
  HQ = 2,
  ARMOUR = 3,
  FOREST = 4,//TODO
  WATER = 5,//TODO
};
byte LEVEL1[] = {
  0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
  0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1,
  0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
  0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};
byte TANK_POS_X = 32;
byte TANK_POS_Y = 56;

const byte startMarker = 0x4E;
const byte actionSetPixel = 1;
const byte actionClear = 2;
const byte actionDrawSprite = 3;

void clearScreen()
{
  Serial.write(startMarker);
  Serial.write(actionClear);
  Serial.write(SCREEN_WIDTH);
  Serial.write(SCREEN_HEIGHT);
}

void drawSprite(unsigned char x, unsigned char y, byte* sprite)
{
  Serial.write(startMarker);
  Serial.write(actionDrawSprite);
  Serial.write(x);
  Serial.write(y);
  for (int i = 0; i < 8; ++i)
  {
    Serial.write(sprite[i]);
  }
}

byte* getSprite(byte type)
{
  switch (type)
  {
    case BRICK:
      return BRICK_SPRITE;
    case HQ:
      return HQ_SPRITE;
    case ARMOUR:
      return ARMOUR_SPRITE;
    default:
      return NULL;
  };
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  clearScreen();
  int idx = 0;
  for (int y = 0; y < TILE_COLUMNS; ++y)
  {
    for (int x = 0; x < TILE_ROWS; ++x)
    {
      byte* sprite = getSprite(LEVEL1[idx]);
      if (sprite)
      {
        drawSprite(x * 8, y * 8, sprite);
      }
      ++idx;
    }
  }
  drawSprite(TANK_POS_X, TANK_POS_Y, TANK_SPRITE);
}

void loop() {
  // put your main code here, to run repeatedly:

}
