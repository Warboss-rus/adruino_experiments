#include "Common.h"
#include "Graphics.h"
#include "Meteo.h"
#include "Terrain.h"
#include "Sound.h"
#include "Input.h"
#include "Tank.h"

unsigned long lastStepTime = 0;
unsigned long lastBulletsTime = 0;
TankManager tanks;
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

bool tryToFixPos(Point& newPos, const Point& prevPos)
{
  if (newPos.x != prevPos.x)
  {
    // try to move down a pixel
    TileType tile1 = Terrain::GetTile(newPos.x / SPRITE_SIZE, (newPos.y + 1) / SPRITE_SIZE);
    TileType tile2 = Terrain::GetTile(newPos.x / SPRITE_SIZE, (newPos.y + SPRITE_SIZE) / SPRITE_SIZE);
    TileType tile3 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + 1) / SPRITE_SIZE);
    TileType tile4 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.y++;
      return true;
    }
    // try to move up a pixel
    tile1 = Terrain::GetTile(newPos.x / SPRITE_SIZE, (newPos.y - 1) / SPRITE_SIZE);
    tile2 = Terrain::GetTile(newPos.x / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 2) / SPRITE_SIZE);
    tile3 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y - 1) / SPRITE_SIZE);
    tile4 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 2) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.y--;
      return true;
    }
  }
  if (newPos.y != prevPos.y)
  {
    // try to move right a pixel
    TileType tile1 = Terrain::GetTile((newPos.x + 1) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    TileType tile2 = Terrain::GetTile((newPos.x + 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    TileType tile3 = Terrain::GetTile((newPos.x + SPRITE_SIZE) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    TileType tile4 = Terrain::GetTile((newPos.x + SPRITE_SIZE) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.x++;
      return true;
    }
    // try to move left a pixel
    tile1 = Terrain::GetTile((newPos.x - 1) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    tile2 = Terrain::GetTile((newPos.x - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    tile3 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 2) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    tile4 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 2) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.x--;
      return true;
    }
  }
  return false;
}

void updateTankPosPlayer(Tank& tank)
{
  updateTankPos(tank, Input::GetPlayerDirection(tank.GetType()));
}

void updateTankPosAI(Tank& tank)
{
  const Point prevPos = tank.GetPos();
  updateTankPos(tank, tank.GetDirection());
  const Point newPos = tank.GetPos();
  if (newPos.x == prevPos.x && newPos.y == prevPos.y)
  {
    tank.SetDirection((Direction)((rand() % 4) + 1));
  }
}

void updateTankPos(Tank& tank, Direction dir)
{
  if (!tank.IsAlive())
  {
    return;
  }
  const Point prevPos = tank.GetPos();
  const Direction prevDir = tank.GetDirection();
  Point newPos = prevPos;
  switch (dir)
  {
    case DIR_LEFT:
      {
        tank.SetDirection(DIR_LEFT);
        if (newPos.x > 0)
        {
          --newPos.x;
        }
      } break;
    case DIR_UP:
      {
        tank.SetDirection(DIR_UP);
        if (newPos.y > 0)
        {
          --newPos.y;
        }
      } break;
    case DIR_RIGHT:
      {
        tank.SetDirection(DIR_RIGHT);
        if (newPos.x + SPRITE_SIZE < SCREEN_WIDTH)
        {
          ++newPos.x;
        }
      } break;
    case DIR_DOWN:
      {
        tank.SetDirection(DIR_DOWN);
        if (newPos.y + SPRITE_SIZE < SCREEN_HEIGHT)
        {
          ++newPos.y;
        }
      } break;
    default:
      return;
  }
  // check for collisions
  TileType tile1 = Terrain::GetTile(newPos.x / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
  TileType tile2 = Terrain::GetTile(newPos.x / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
  TileType tile3 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
  TileType tile4 = Terrain::GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
  if ((tile1 != NONE) || (tile2 != NONE) || (tile3 != NONE) || (tile4 != NONE))
  {
    if (!tryToFixPos(newPos, prevPos))
    {
      newPos.x = prevPos.x;
      newPos.y = prevPos.y;
    }
  }
  tank.SetPosition(newPos);
  if (tanks.CollidesWithTanks(tank))
  {
    tank.SetPosition(prevPos);
  }
  if (prevPos.x != tank.GetPos().x || prevPos.y != tank.GetPos().y || prevDir != tank.GetDirection())
  {
    Graphics::ClearMovingSprite(prevPos, tank.GetPos());
    Graphics::DrawTank(tank.GetPos(), tank.GetDirection(), tank.GetType());
  }
}

void updateTankFire(Tank& tank)
{
  if (!tank.IsAlive())
  {
    return;
  }
  if (tank.IsReloading())
  {
    tank.Reload();
  }
  if (!tank.IsReloading() && ((tank.GetType() == TANK_ENEMY) || Input::GetFireButtonPressed(tank.GetType())))
  {
    tank.StartReload();
    const Point tankPos = tank.GetPos();
    for (size_t i = 0; i < MAX_BULLETS; ++i)
    {
      Bullet& bullet = bullets[i];
      if (!bullet.state)
      {
        bullet.pos.x = tankPos.x + SPRITE_SIZE / 2 - BULLET_SIZE / 2;
        bullet.pos.y = tankPos.y + SPRITE_SIZE / 2 - BULLET_SIZE / 2;
        switch (tank.GetDirection())
        {
          case DIR_UP:
            bullet.pos.y = tankPos.y - 1;
            break;
          case DIR_DOWN:
            bullet.pos.y = tankPos.y + SPRITE_SIZE + 1;
            break;
          case DIR_LEFT:
            bullet.pos.x = tankPos.x - 1;
            break;
          case DIR_RIGHT:
            bullet.pos.x = tankPos.x + SPRITE_SIZE + 1;
            break;
        }
        bullet.dir = tank.GetDirection();
        bullet.owner = tank.GetType();
        bullet.state = BULLET_STATE_FLYING;
        Graphics::DrawBullet(bullet.pos);
        Sound::FireSound();
        return;
      }
    }
  }
}

bool updateBullet(Bullet& bullet)
{
  if ((bullet.dir == DIR_LEFT && bullet.pos.x == 0) || (bullet.dir == DIR_RIGHT && bullet.pos.x == SCREEN_WIDTH - 1) || (bullet.dir == DIR_UP && bullet.pos.y == 0) || (bullet.dir == DIR_DOWN && bullet.pos.y == SCREEN_HEIGHT - 1))
  {
    bullet.state = 0;
    return false;
  }
  switch (bullet.dir)
  {
    case DIR_LEFT:
      --bullet.pos.x;
      break;
    case DIR_RIGHT:
      ++bullet.pos.x;
      break;
    case DIR_UP:
      --bullet.pos.y;
      break;
    case DIR_DOWN:
      ++bullet.pos.y;
      break;
  }
  TileType tile = Terrain::GetTile(bullet.pos.x / SPRITE_SIZE, bullet.pos.y / SPRITE_SIZE);
  if (tile == ARMOUR)
  {
    bullet.state = 0;
    return false;
  }
  if (tile == BRICK || tile == HQ)
  {
    Terrain::ClearTile(bullet.pos.x / SPRITE_SIZE, bullet.pos.y / SPRITE_SIZE);
    bullet.pos.x = bullet.pos.x / SPRITE_SIZE * SPRITE_SIZE;
    bullet.pos.y = bullet.pos.y / SPRITE_SIZE * SPRITE_SIZE;
    bullet.state = EXPLOSION_FRAMES;
    Graphics::DrawExplosion(bullet.pos.x, bullet.pos.y, bullet.state);
    Sound::ExplosionSound();
    if (tile == HQ)
    {
      endGame();
    }
    return false;
  }
  if (Tank* collidedTank = tanks.CollidesWithTanks(bullet))
  {
    collidedTank->Kill();
    bullet.pos = collidedTank->GetPos();
    bullet.state = EXPLOSION_FRAMES;
    Graphics::DrawExplosion(bullet.pos.x, bullet.pos.y, bullet.state);
    return false;
  }
  for (size_t i = 0; i < MAX_BULLETS; ++i)
  {
    Bullet& otherBullet = bullets[i];
    if ((otherBullet.state == BULLET_STATE_FLYING) && (otherBullet.owner != bullet.owner) &&
        (bullet.pos.x + BULLET_SIZE >= otherBullet.pos.x) && (bullet.pos.x <= otherBullet.pos.x + BULLET_SIZE) && (bullet.pos.y + BULLET_SIZE >= otherBullet.pos.y) && (bullet.pos.y <= otherBullet.pos.y + BULLET_SIZE))
    {
      bullet.state = 0;
      otherBullet.state = 0;
      Graphics::ClearBullet(bullet.pos);
      Graphics::ClearBullet(otherBullet.pos);
      return false;
    }
  }
  return true;
}

void clearBullet(Point pos)
{
  TileType tile1 = Terrain::GetTile(pos.x / SPRITE_SIZE, pos.y / SPRITE_SIZE);
  TileType tile2 = Terrain::GetTile(pos.x / SPRITE_SIZE, (pos.y + BULLET_SIZE - 1) / SPRITE_SIZE);
  TileType tile3 = Terrain::GetTile((pos.x + BULLET_SIZE - 1) / SPRITE_SIZE, pos.y / SPRITE_SIZE);
  TileType tile4 = Terrain::GetTile((pos.x + BULLET_SIZE - 1) / SPRITE_SIZE, (pos.y + BULLET_SIZE - 1) / SPRITE_SIZE);
  if (tile1 == NONE && tile2 == NONE && tile3 == NONE && tile4 == NONE)
  {
    return Graphics::ClearBullet(pos);
  }
  drawTerrain(tile1, pos.x / SPRITE_SIZE * SPRITE_SIZE, pos.y / SPRITE_SIZE * SPRITE_SIZE);
  if (((pos.y + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE) != (pos.y / SPRITE_SIZE * SPRITE_SIZE))
  {
    drawTerrain(tile2, pos.x / SPRITE_SIZE * SPRITE_SIZE, (pos.y + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE);
  }
  if (((pos.x + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE) != (tile1, pos.x / SPRITE_SIZE * SPRITE_SIZE))
  {
    drawTerrain(tile3, (pos.x + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE, pos.y / SPRITE_SIZE * SPRITE_SIZE);
  }
  if ((((pos.y + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE) != (pos.y / SPRITE_SIZE * SPRITE_SIZE)) || (((pos.x + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE) != (tile1, pos.x / SPRITE_SIZE * SPRITE_SIZE)))
  {
    drawTerrain(tile4, (pos.x + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE, (pos.y + BULLET_SIZE - 1) / SPRITE_SIZE * SPRITE_SIZE);
  }
}

void updateBullets()
{
  for (size_t i = 0; i < MAX_BULLETS; ++i)
  {
    Bullet& bullet = bullets[i];
    if (bullet.state == BULLET_STATE_FLYING)
    {
      // remove bullet at previous pos
      clearBullet(bullet.pos);
      if (updateBullet(bullet))
      {
        Graphics::DrawBullet(bullet.pos);
      }
    }
    else if (bullet.state != 0)
    {
      if (--bullet.state == 0)
      {
        Graphics::ClearSprite(bullet.pos);
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

void endGame()
{
  gameOver = true;
  Graphics::DrawText(54, 72, "Game");
  Graphics::DrawText(54, 80, "Over");
}

void setup() {
  // put your setup code here, to run once:
  Input::Setup();
  srand(analogRead(PB9));
  Graphics::InitScreen();
  drawLevel();
  tanks.Respawn();
  Meteo::Setup();

  Sound::Setup();
  Sound::PlayIntroMusic();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!gameOver)
  {
    const unsigned long timePassed = millis();
    if (timePassed >= lastStepTime + TANK_STEP_DURATION)
    {
      lastStepTime = timePassed;
      tanks.Respawn();
      updateTankPosPlayer(tanks.GetPlayer1());
      updateTankFire(tanks.GetPlayer1());
      updateTankPosPlayer(tanks.GetPlayer2());
      updateTankFire(tanks.GetPlayer2());
      updateTankPosAI(tanks.GetEnemy1());
      updateTankPosAI(tanks.GetEnemy2());
      updateTankFire(tanks.GetEnemy1());
      updateTankFire(tanks.GetEnemy2());
    }
    if (timePassed >= lastBulletsTime + BULLET_STEP_DURATION)
    {
      lastBulletsTime = timePassed;
      updateBullets();
    }
    if (tanks.PlayersAreDead())
    {
      endGame();
    }
  }
  Sound::Update();
  Meteo::Update();
}
