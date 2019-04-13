#define TANK_STEP_DURATION 100
#define BULLET_STEP_DURATION 40
#define RELOAD_FRAMES 40
#define MAX_BULLETS 10
#define BULLET_STATE_FLYING 255
#define EXPLOSION_FRAMES 5
#define JOYSTICK_X_PIN PA0
#define JOYSTICK_Y_PIN PA1
#define JOYSTICK_BUTTON_PIN PA2
#define JOYSTICK2_X_PIN PB0
#define JOYSTICK2_Y_PIN PB1
#define JOYSTICK2_BUTTON_PIN PB10
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
  TankIndex type;
  byte lives;
};

struct Bullet
{
  byte x;
  byte y;
  Direction dir;
  TankIndex owner;
  byte state; // 0 - disabled, 1-EXPLOSION_FRAMES - explosion, 255 - flying
};

unsigned long lastStepTime = 0;
unsigned long lastBulletsTime = 0;
Tank tankPos = {40, 152, DIR_UP, 0, TANK_P1, 1};
Tank tankPos2 = {72, 152, DIR_UP, 0, TANK_P2, 1};
Tank tankEnemy1 = {0, 0, DIR_RIGHT, 0, TANK_ENEMY, 1};
Tank tankEnemy2 = {120, 0, DIR_DOWN, 0, TANK_ENEMY, 1};
Bullet bullets[MAX_BULLETS] = {};
bool gameOver = false;

void drawTerrain(byte type, byte x, byte y)
{
  switch (type)
  {
    case BRICK:
      return Graphics::DrawBricks(x, y);
    case HQ:
      return Graphics::DrawHQ(x, y);
    case ARMOUR:
      return Graphics::DrawArmour(x, y);
    case TREE:
      return Graphics::DrawTrees(x, y);
    case WATER:
      return Graphics::DrawWater(x, y);
  };
}

Direction getCurrentDirection(uint32_t xPin, uint32_t yPin)
{
  const int x = analogRead(xPin);
  const int y = analogRead(yPin);
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

void updateTankPos(Tank& tankPos, uint32_t xPin, uint32_t yPin)
{
  Tank prevPos = tankPos;
  switch (getCurrentDirection(xPin, yPin))
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
    Graphics::DrawTank(tankPos.x, tankPos.y, tankPos.dir, tankPos.type);
  }
}

void updateTankFire(Tank& tankPos, uint32_t buttonPin)
{
  if (tankPos.reload > 0)
  {
    --tankPos.reload;
  }
  if ((tankPos.reload == 0) && (!digitalRead(buttonPin)))
  {
    tankPos.reload = RELOAD_FRAMES;
    for (size_t i = 0; i < MAX_BULLETS; ++i)
    {
      Bullet& bulletPos = bullets[i];
      if (!bulletPos.state)
      {
        bulletPos.x = tankPos.x + SPRITE_SIZE / 2;
        bulletPos.y = tankPos.y + SPRITE_SIZE / 2;
        bulletPos.dir = tankPos.dir;
        bulletPos.owner = tankPos.type;
        bulletPos.state = BULLET_STATE_FLYING;
        Graphics::DrawBullet(bulletPos.x, bulletPos.y);
        return;
      }
    }
  }
}

bool collideWithTank(Bullet& bullet, Tank& tank)
{
  if (bullet.owner != tank.type && bullet.x >= tank.x && bullet.x < tank.x + SPRITE_SIZE && bullet.y >= tank.y && bullet.y < tank.y + SPRITE_SIZE && tank.lives > 0)
  {
    --tank.lives;
    bullet.x = tank.x;
    bullet.y = tank.y;
    bullet.state = EXPLOSION_FRAMES;
    Graphics::DrawExplosion(bullet.x, bullet.y, bullet.state);
    return true;
  }
  return false;
}

bool updateBullet(Bullet& bullet)
{
  if ((bullet.dir == DIR_LEFT && bullet.x == 0) || (bullet.dir == DIR_RIGHT && bullet.x == SCREEN_WIDTH - 1) || (bullet.dir == DIR_UP && bullet.y == 0) || (bullet.dir == DIR_DOWN && bullet.y == SCREEN_HEIGHT - 1))
  {
    bullet.state = 0;
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
  TileType tile = Terrain::GetTile(bullet.x / SPRITE_SIZE, bullet.y / SPRITE_SIZE);
  if (tile == ARMOUR)
  {
    bullet.state = false;
    return false;
  }
  if (tile == BRICK || tile == HQ)
  {
    Terrain::ClearTile(bullet.x / SPRITE_SIZE, bullet.y / SPRITE_SIZE);
    bullet.x = bullet.x / SPRITE_SIZE * SPRITE_SIZE;
    bullet.y = bullet.y / SPRITE_SIZE * SPRITE_SIZE;
    bullet.state = EXPLOSION_FRAMES;
    Graphics::DrawExplosion(bullet.x, bullet.y, bullet.state);
    if (tile == HQ)
    {
      gameOver = true;
      Graphics::DrawText(54, 72, "Game");
      Graphics::DrawText(54, 80, "Over");
    }
    return false;
  }
  if (collideWithTank(bullet, tankPos) || collideWithTank(bullet, tankPos2) || collideWithTank(bullet, tankEnemy1) || collideWithTank(bullet, tankEnemy2))
  {
    return false;
  }
  return true;
}

void updateBullets()
{
  for (size_t i = 0; i < MAX_BULLETS; ++i)
  {
    Bullet& bullet = bullets[i];
    if (bullet.state == BULLET_STATE_FLYING)
    {
      // remove bullet at previous pos
      Graphics::ClearBullet(bullet.x, bullet.y);
      if (updateBullet(bullet))
      {
        Graphics::DrawBullet(bullet.x, bullet.y);
      }
    }
    else if (bullet.state != 0)
    {
      if (--bullet.state == 0)
      {
        Graphics::ClearSprite(bullet.x, bullet.y);
      }
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
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK2_BUTTON_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_X_PIN, INPUT_ANALOG);
  pinMode(JOYSTICK_Y_PIN, INPUT_ANALOG);
  pinMode(JOYSTICK2_X_PIN, INPUT_ANALOG);
  pinMode(JOYSTICK2_Y_PIN, INPUT_ANALOG);
  Graphics::InitScreen();
  drawLevel();
  Graphics::DrawTank(tankPos.x, tankPos.y, tankPos.dir, tankPos.type);
  Graphics::DrawTank(tankPos2.x, tankPos2.y, tankPos2.dir, tankPos2.type);
  Graphics::DrawTank(tankEnemy1.x, tankEnemy1.y, tankEnemy1.dir, tankEnemy1.type);
  Graphics::DrawTank(tankEnemy2.x, tankEnemy2.y, tankEnemy2.dir, tankEnemy2.type);

  setupMeteo();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!gameOver)
  {
    const unsigned long timePassed = millis();
    if (timePassed >= lastStepTime + TANK_STEP_DURATION)
    {
      lastStepTime = timePassed;
      updateTankPos(tankPos, JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
      updateTankFire(tankPos, JOYSTICK_BUTTON_PIN);
      updateTankPos(tankPos2, JOYSTICK2_X_PIN, JOYSTICK2_Y_PIN);
      updateTankFire(tankPos2, JOYSTICK2_BUTTON_PIN);
    }
    if (timePassed >= lastBulletsTime + BULLET_STEP_DURATION)
    {
      lastBulletsTime = timePassed;
      updateBullets();
    }
  }
  updateMeteo();
}
