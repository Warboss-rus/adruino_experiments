#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SPRITE_SIZE 8
#define BULLET_SIZE 2
#define TILE_ROWS 16 // SCREEN_WIDTH / SPRITE_SIZE
#define TILE_COLUMNS 8 // SCREEN_HEIGHT / SPRITE_SIZE
#define TANK_STEP_DURATION 125
#define BULLET_STEP_DURATION 50
#define RELOAD_FRAMES 40
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_BUTTON_PIN A2
#define JOYSTICK_DEADZONE_MIN 256
#define JOYSTICK_DEADZONE_MAX 768
#define JOYSTICK_NEUTRAL 512

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

const byte TANK_SPRITE[] = {
  B00001000,
  B11001011,
  B01001010,
  B11111111,
  B01111110,
  B11111111,
  B01111110,
  B11011011,
};
const byte TANK_SPRITE_LEFT[] = {
  B10101010,
  B11111110,
  B01111000,
  B11111000,
  B11111111,
  B01111000,
  B11111110,
  B10101010,
};
const byte TANK_SPRITE_DOWN[] = {
  B11011011,
  B01111110,
  B11111111,
  B01111110,
  B11111111,
  B01010010,
  B11010011,
  B00010000,
};
const byte TANK_SPRITE_RIGHT[] = {
  B01010101,
  B01111111,
  B00011110,
  B11111111,
  B00011111,
  B00011110,
  B01111111,
  B01010101,
};
const byte BRICK_SPRITE[] = {
  B10111011,
  B10111011,
  B00000000,
  B11101110,
  B11101110,
  B00000000,
  B10111011,
  B10111011,
};
const byte HQ_SPRITE[] = {
  B11011011,
  B01010010,
  B01111110,
  B00111100,
  B00111100,
  B00011000,
  B00111100,
  B01011010,
};
const byte ARMOUR_SPRITE[] = {
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
const byte LEVEL1[] PROGMEM = {
  0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
  0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1,
  0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
  0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};
unsigned long lastStepTime = 0;
unsigned long lastBulletsTime = 0;
unsigned long lastTempUpdateTime = 0;
Tank tankPos = {32, 56, DIR_UP, 0};
Bullet bulletPos = {0, 0, DIR_UP, false};

#include <U8g2lib.h>
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, 10);

void clearScreen()
{
  u8g2.clearBuffer();
}

void drawSprite(byte x, byte y, const byte* sprite)
{
  u8g2.setColorIndex(1);
  u8g2.drawXBM(x, y, SPRITE_SIZE, SPRITE_SIZE, sprite);
}

void clearSprite(byte x, byte y)
{
  u8g2.setColorIndex(0);
  u8g2.drawBox(x, y, SPRITE_SIZE, SPRITE_SIZE);
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
    default:
      return NULL;
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
    drawSprite(tankPos.x, tankPos.y, getTankSprite(tankPos.dir));
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
  u8g2.setColorIndex(1);
  u8g2.drawBox(x - BULLET_SIZE / 2, y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE);
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
    u8g2.setColorIndex(0);
    u8g2.drawBox(bulletPos.x - BULLET_SIZE / 2, bulletPos.y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE);
    if (updateBullet(bulletPos))
    {
      drawBullet(bulletPos.x, bulletPos.y);
    }
    return true;
  }
  return false;
}

#include <OneWire.h>
OneWire ds(A5);
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

    float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
    unsigned short intTemp = temperature * 100;
    Serial.write((byte)intTemp);
    Serial.write((byte)(intTemp >> 8));
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);
  Serial.begin(115200);
  u8g2.begin();
  clearScreen();
  int idx = 0;
  for (int y = 0; y < TILE_COLUMNS; ++y)
  {
    for (int x = 0; x < TILE_ROWS; ++x)
    {
      const byte* sprite = getSprite(pgm_read_byte_near(LEVEL1 + idx));
      if (sprite)
      {
        drawSprite(x * SPRITE_SIZE, y * SPRITE_SIZE, sprite);
      }
      ++idx;
    }
  }
  drawSprite(tankPos.x, tankPos.y, getTankSprite(tankPos.dir));
}

void loop() {
  // put your main code here, to run repeatedly:
  const unsigned long timePassed = millis();
  if (timePassed >= lastStepTime + TANK_STEP_DURATION)
  {
    lastStepTime = timePassed;
    updateTankPos(tankPos);
    updateTankFire(tankPos);
    u8g2.sendBuffer();
  }
  if (timePassed >= lastBulletsTime + BULLET_STEP_DURATION)
  {
    lastBulletsTime = timePassed;
    if (updateBullets())
    {
      u8g2.sendBuffer();
    }
  }
  updateTemp();
}
