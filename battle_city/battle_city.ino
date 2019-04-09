#define TANK_STEP_DURATION 125
#define BULLET_STEP_DURATION 50
#define RELOAD_FRAMES 40
#define JOYSTICK_X_PIN PA0
#define JOYSTICK_Y_PIN PA1
#define JOYSTICK_BUTTON_PIN PA2
#define JOYSTICK_DEADZONE_MIN 1024
#define JOYSTICK_DEADZONE_MAX 3072
#define JOYSTICK_NEUTRAL 2048

#include "Common.h"
#include "Graphics.h"
#include "Meteo.h"
#include "Terrain.h"

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

unsigned long lastStepTime = 0;
unsigned long lastBulletsTime = 0;
Tank tankPos = {40, 152, DIR_UP, 0};
Tank tankPos2 = {72, 152, DIR_UP, 0};
Bullet bulletPos = {0, 0, DIR_UP, false};

void drawTerrain(byte type, byte x, byte y)
{
  switch (type)
  {
    case BRICK:
      return Graphics::DrawBricks(x, y);
    case HQ:
      return Graphics::DrawBricks(x, y);
    case ARMOUR:
      return Graphics::DrawArmour(x, y);
    case TREE:
      return Graphics::DrawTrees(x, y);
    case WATER:
      return Graphics::DrawWater(x, y);
  };
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
  TileType tile1 = Terrain::GetTile(tankPos.x / SPRITE_SIZE, tankPos.y / SPRITE_SIZE);
  TileType tile2 = Terrain::GetTile(tankPos.x / SPRITE_SIZE, (tankPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
  TileType tile3 = Terrain::GetTile((tankPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, tankPos.y / SPRITE_SIZE);
  TileType tile4 = Terrain::GetTile((tankPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (tankPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
  if ((tile1 != NONE) || (tile2 != NONE) || (tile3 != NONE) || (tile4 != NONE))
  {
    tankPos.x = prevPos.x;
    tankPos.y = prevPos.y;
  }
  if (prevPos.x != tankPos.x || prevPos.y != tankPos.y || prevPos.dir != tankPos.dir)
  {
    Graphics::ClearSprite(prevPos.x, prevPos.y);
    Graphics::DrawTank(tankPos.x, tankPos.y, tankPos.dir, TANK_P1);
  }
}

void updateTankFire(Tank& tankPos)
{
  if (tankPos.reload > 0)
  {
    --tankPos.reload;
  }
  if ((tankPos.reload == 0) && (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) && (!bulletPos.active)) 
  {
    tankPos.reload = RELOAD_FRAMES;
    bulletPos.x = tankPos.x + SPRITE_SIZE / 2;
    bulletPos.y = tankPos.y + SPRITE_SIZE / 2;
    bulletPos.dir = tankPos.dir;
    bulletPos.active = true;
    Graphics::DrawBullet(bulletPos.x, bulletPos.y);
  }
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
  if (Terrain::GetTile(bullet.x / SPRITE_SIZE, bullet.y / SPRITE_SIZE) != NONE)
  {
    bullet.active = false;
    return false;
  }
  return true;
}

void updateBullets()
{
  if (bulletPos.active)
  {
    // remove bullet at previous pos
    Graphics::ClearBullet(bulletPos.x, bulletPos.y);
    if (updateBullet(bulletPos))
    {
      Graphics::DrawBullet(bulletPos.x, bulletPos.y);
    }
  }
}

void drawLevel()
{
  Graphics::ClearScreen();
  for (int y = 0; y < TILE_COLUMNS; ++y)
  {
    for (int x = 0; x < TILE_ROWS; ++x)
    {
      if (byte type = Terrain::GetTile(x, y))
      {
        drawTerrain(type, x * SPRITE_SIZE, y * SPRITE_SIZE);
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);
  Graphics::InitScreen();
  drawLevel();
  Graphics::DrawTank(tankPos.x, tankPos.y, tankPos.dir, TANK_P1);
  Graphics::DrawTank(tankPos2.x, tankPos2.y, tankPos2.dir, TANK_P2);
  Graphics::DrawTank(0, 0, DIR_RIGHT, TANK_ENEMY);
  Graphics::DrawTank(120, 0, DIR_DOWN, TANK_ENEMY);

  setupMeteo();
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
  updateMeteo();
}
