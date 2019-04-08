#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define SPRITE_SIZE 8
#define BULLET_SIZE 2
#define TILE_ROWS 16 // SCREEN_WIDTH / SPRITE_SIZE
#define TILE_COLUMNS 20 // SCREEN_HEIGHT / SPRITE_SIZE
#define TANK_STEP_DURATION 125
#define BULLET_STEP_DURATION 50
#define RELOAD_FRAMES 40
#define JOYSTICK_X_PIN PA0
#define JOYSTICK_Y_PIN PA1
#define JOYSTICK_BUTTON_PIN PA2
#define JOYSTICK_DEADZONE_MIN 1024
#define JOYSTICK_DEADZONE_MAX 3072
#define JOYSTICK_NEUTRAL 2048

enum Direction
{
  DIR_NONE,
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
};

struct Tank
{
  byte x;
  byte y;
  Direction dir;
  byte reload;
};
struct Bullet
{
  byte x;
  byte y;
  Direction dir;
  bool active;
};

const byte TANK_SPRITE[] PROGMEM  = {
  B00001000,
  B11001011,
  B01001010,
  B11111111,
  B01111110,
  B11111111,
  B01111110,
  B11011011,
};
const byte TANK_SPRITE_RIGHT[] PROGMEM  = {
  B10101010,
  B11111110,
  B01111000,
  B11111000,
  B11111111,
  B01111000,
  B11111110,
  B10101010,
};
const byte TANK_SPRITE_DOWN[] PROGMEM  = {
  B11011011,
  B01111110,
  B11111111,
  B01111110,
  B11111111,
  B01010010,
  B11010011,
  B00010000,
};
const byte TANK_SPRITE_LEFT[] PROGMEM  = {
  B01010101,
  B01111111,
  B00011110,
  B11111111,
  B00011111,
  B00011110,
  B01111111,
  B01010101,
};
const byte BRICK_SPRITE[] PROGMEM  = {
  B10111011,
  B10111011,
  B00000000,
  B11101110,
  B11101110,
  B00000000,
  B10111011,
  B10111011,
};
const byte HQ_SPRITE[] PROGMEM  = {
  B11011011,
  B01010010,
  B01111110,
  B00111100,
  B00111100,
  B00011000,
  B00111100,
  B01011010,
};
const byte ARMOUR_SPRITE[] PROGMEM  = {
  B11101110,
  B11101110,
  B11101110,
  B00000000,
  B11101110,
  B11101110,
  B11101110,
  B00000000,
};
const byte WATER_SPRITE[] PROGMEM = {
  B11111110,
  B10111111,
  B11011111,
  B11101101,
  B11111110,
  B11101111,
  B11010111,
  B01111111,
};
const byte TREE_SPRITE[] PROGMEM = {
  B00000010,
  B00010101,
  B00000111,
  B00011001,
  B00101110,
  B00011111,
  B11111011,
  B01101110,
};
const uint16_t BRICK_FRONT_COLOR = 0x9A40; //0x9c4a00
const uint16_t BRICK_BACK_COLOR = 0x630C; //0x636363
const uint16_t P1_TANK_COLOR = 0xE4E4; //0xe79c21
const uint16_t P2_TANK_COLOR = 0x0466; //0x008c31
const uint16_t HQ_COLOR = 0x630C; //0x636363
const uint16_t ARMOUR_FRONT_COLOR = 0xFFFF; //0xFFFFFF
const uint16_t ARMOUR_BACK_COLOR = 0x630C; //0x636363
const uint16_t BULLET_COLOR = 0xAD75; //0xadadad
const uint16_t WATER_FRONT_COLOR = 0x421F; //0x4242ff
const uint16_t WATER_BACK_COLOR = 0xB77D; //0xb5efef
const uint16_t TREE_FRONT_COLOR = 0x8EA0; //0x8cd600
const uint16_t TREE_BACK_COLOR = 0x0281; //0x005208

enum TileType
{
  NONE = 0,
  BRICK = 1,
  HQ = 2,
  ARMOUR = 3,
  TREE = 4,
  WATER = 5,
};
const byte LEVEL1[] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 1, 0, 1, 0, 4, 5, 4, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 5, 5, 5, 0, 1, 0, 1, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 4, 5, 4, 0, 0, 0, 0, 0, 0, 0,
  3, 3, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 3,
  0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 1, 3, 1, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
  3, 3, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 3,
  0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0,
  1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0,
};
unsigned long lastStepTime = 0;
unsigned long lastBulletsTime = 0;
unsigned long lastTempUpdateTime = 0;
Tank tankPos = {40, 152, DIR_UP, 0};
Bullet bulletPos = {0, 0, DIR_UP, false};

#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h> // hardware-specific library
Adafruit_ST7735 tft(PB5, PB3, PB4);

void clearScreen()
{
  tft.fillScreen(ST7735_BLACK);
}

void drawSprite(byte x, byte y, const byte* sprite, uint16_t frontColor, uint16_t backColor)
{
  tft.drawBitmap(x, y, sprite, SPRITE_SIZE, SPRITE_SIZE, frontColor, backColor);
}

void drawTransparentSprite(byte x, byte y, const byte* sprite, uint16_t color)
{
  tft.drawBitmap(x, y, sprite, SPRITE_SIZE, SPRITE_SIZE, color);
}

void clearSprite(byte x, byte y)
{
  tft.fillRect(x, y, SPRITE_SIZE, SPRITE_SIZE, ST7735_BLACK);
}

const byte* getSprite(byte type)
{
  switch (type)
  {
    case BRICK:
      return BRICK_SPRITE;
    case HQ:
      return HQ_SPRITE;
    case ARMOUR:
      return ARMOUR_SPRITE;
    case TREE:
      return TREE_SPRITE;
    case WATER:
      return WATER_SPRITE;
    default:
      return NULL;
  };
}

const uint16_t getFrontColor(byte type)
{
  switch (type)
  {
    case BRICK:
      return BRICK_FRONT_COLOR;
    case HQ:
      return HQ_COLOR;
    case ARMOUR:
      return ARMOUR_FRONT_COLOR;
    case TREE:
      return TREE_FRONT_COLOR;
    case WATER:
      return WATER_FRONT_COLOR;
    default:
      return ST7735_BLACK;
  };
}

const uint16_t getBackColor(byte type)
{
  switch (type)
  {
    case BRICK:
      return BRICK_BACK_COLOR;
    case ARMOUR:
      return ARMOUR_BACK_COLOR;
    case TREE:
      return TREE_BACK_COLOR;
    case WATER:
      return WATER_BACK_COLOR;
    default:
      return ST7735_BLACK;
  };
}

const byte* getTankSprite(Direction dir)
{
  switch (dir)
  {
    case DIR_UP:
      return TANK_SPRITE;
    case DIR_DOWN:
      return TANK_SPRITE_DOWN;
    case DIR_LEFT:
      return TANK_SPRITE_LEFT;
    case DIR_RIGHT:
      return TANK_SPRITE_RIGHT;
  }
  return NULL;
}

Direction getCurrentDirection()
{
  const int x = analogRead(JOYSTICK_X_PIN);
  const int y = analogRead(JOYSTICK_Y_PIN);
  const int deltaX = abs(x - JOYSTICK_NEUTRAL);
  const int deltaY = abs(y - JOYSTICK_NEUTRAL);
  if (deltaX > deltaY)
  {
    if (x <= JOYSTICK_DEADZONE_MIN)
    {
      return DIR_LEFT;
    }
    else if (x >= JOYSTICK_DEADZONE_MAX)
    {
      return DIR_RIGHT;
    }
  }
  else
  {
    if (y <= JOYSTICK_DEADZONE_MIN)
    {
      return DIR_UP;
    }
    else if (y >= JOYSTICK_DEADZONE_MAX)
    {
      return DIR_DOWN;
    }
  }
  return DIR_NONE;
}

void updateTankPos(Tank& tankPos)
{
  Tank prevPos = tankPos;
  switch (getCurrentDirection())
  {
    case DIR_LEFT:
      {
        tankPos.dir = DIR_LEFT;
        if (tankPos.x > 0)
        {
          --tankPos.x;
        }
      } break;
    case DIR_UP:
      {
        tankPos.dir = DIR_UP;
        if (tankPos.y > 0)
        {
          --tankPos.y;
        }
      } break;
    case DIR_RIGHT:
      {
        tankPos.dir = DIR_RIGHT;
        if (tankPos.x + SPRITE_SIZE < SCREEN_WIDTH)
        {
          ++tankPos.x;
        }
      } break;
    case DIR_DOWN:
      {
        tankPos.dir = DIR_DOWN;
        if (tankPos.y + SPRITE_SIZE < SCREEN_HEIGHT)
        {
          ++tankPos.y;
        }
      } break;
    default:
      return;
  }
  // check for collisions
  int idx1 = tankPos.x / SPRITE_SIZE + (tankPos.y / SPRITE_SIZE * TILE_ROWS);
  int idx2 = tankPos.x / SPRITE_SIZE + ((tankPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE * TILE_ROWS);
  int idx3 = (tankPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE + (tankPos.y / SPRITE_SIZE * TILE_ROWS);
  int idx4 = (tankPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE + ((tankPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE * TILE_ROWS);
  if ((pgm_read_byte_near(LEVEL1 + idx1) != NONE) || (pgm_read_byte_near(LEVEL1 + idx2) != NONE) || (pgm_read_byte_near(LEVEL1 + idx3) != NONE) || (pgm_read_byte_near(LEVEL1 + idx4) != NONE))
  {
    tankPos.x = prevPos.x;
    tankPos.y = prevPos.y;
  }
  if (prevPos.x != tankPos.x || prevPos.y != tankPos.y || prevPos.dir != tankPos.dir)
  {
    clearSprite(prevPos.x, prevPos.y);
    drawTransparentSprite(tankPos.x, tankPos.y, getTankSprite(tankPos.dir), P1_TANK_COLOR);
  }
}

void updateTankFire(Tank& tankPos)
{
  if (tankPos.reload > 0)
  {
    --tankPos.reload;
  }
  if ((tankPos.reload == 0) && (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) && (!bulletPos.active)) {
    tankPos.reload = RELOAD_FRAMES;
    bulletPos.x = tankPos.x + SPRITE_SIZE / 2;
    bulletPos.y = tankPos.y + SPRITE_SIZE / 2;
    bulletPos.dir = tankPos.dir;
    bulletPos.active = true;
    drawBullet(bulletPos.x, bulletPos.y);
  }
}

void drawBullet(byte x, byte y)
{
  tft.fillRect(x - BULLET_SIZE / 2, y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE, BULLET_COLOR);
}

bool updateBullet(Bullet& bullet)
{
  if ((bullet.dir == DIR_LEFT && bullet.x == 0) || (bullet.dir == DIR_RIGHT && bullet.x == SCREEN_WIDTH - 1) || (bullet.dir == DIR_UP && bullet.y == 0) || (bullet.dir == DIR_DOWN && bullet.y == SCREEN_HEIGHT - 1))
  {
    bullet.active = false;
    return false;
  }
  switch (bullet.dir)
  {
    case DIR_LEFT:
      --bullet.x;
      break;
    case DIR_RIGHT:
      ++bullet.x;
      break;
    case DIR_UP:
      --bullet.y;
      break;
    case DIR_DOWN:
      ++bullet.y;
      break;
  }
  int idx = bullet.x / SPRITE_SIZE + (bullet.y / SPRITE_SIZE * TILE_ROWS);
  if (pgm_read_byte_near(LEVEL1 + idx) != NONE)
  {
    bullet.active = false;
    return false;
  }
  return true;
}

bool updateBullets()
{
  if (bulletPos.active)
  {
    // remove bullet at previous pos
    tft.fillRect(bulletPos.x - BULLET_SIZE / 2, bulletPos.y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE, ST7735_BLACK);
    if (updateBullet(bulletPos))
    {
      drawBullet(bulletPos.x, bulletPos.y);
    }
    return true;
  }
  return false;
}

#include <OneWire.h>
#include <Adafruit_BMP085_U.h>
#include <nRF24L01.h>
#include <RF24.h>

OneWire ds(PA3);
Adafruit_BMP085_Unified  bmp;
RF24 radio(PB0, PA4); // CE, CSN

struct MeteoState
{
  float temperature;
  float humidity;
  unsigned int pressure;
};
#define METEO_STATE_DATA_SIZE 12
const byte address[6] = "12345";

unsigned int updateTemp()
{
  byte data[2];
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);

  if (millis() - lastTempUpdateTime > 1000)
  {
    lastTempUpdateTime = millis();
    ds.reset();
    ds.write(0xCC);
    ds.write(0xBE);

    data[0] = ds.read();
    data[1] = ds.read();

    MeteoState state = {0.0f, 0.0f, 0};
    state.temperature = ((data[1] << 8) | data[0]) * 0.0625;
    float P = 0;
    bmp.getPressure(&P);
    state.pressure = (unsigned int)P;
    radio.write(&state, METEO_STATE_DATA_SIZE);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);
  Serial.begin(115200);
  tft.initR (INITR_BLACKTAB);
  clearScreen();
  int idx = 0;
  for (int y = 0; y < TILE_COLUMNS; ++y)
  {
    for (int x = 0; x < TILE_ROWS; ++x)
    {
      byte type = pgm_read_byte_near(LEVEL1 + idx);
      const byte* sprite = getSprite(type);
      if (sprite)
      {
        drawSprite(x * SPRITE_SIZE, y * SPRITE_SIZE, sprite, getFrontColor(type), getBackColor(type));
      }
      ++idx;
    }
  }
  drawTransparentSprite(tankPos.x, tankPos.y, getTankSprite(tankPos.dir), P1_TANK_COLOR);

  bmp.begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  const unsigned long timePassed = millis();
  if (timePassed >= lastStepTime + TANK_STEP_DURATION)
  {
    lastStepTime = timePassed;
    updateTankPos(tankPos);
    updateTankFire(tankPos);
  }
  if (timePassed >= lastBulletsTime + BULLET_STEP_DURATION)
  {
    lastBulletsTime = timePassed;
    updateBullets();
  }
  updateTemp();
}
