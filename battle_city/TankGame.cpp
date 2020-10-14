#include "TankGame.h"
#include <Arduino.h>
#include "TankGraphics.h"
#include "Input.h"
#include "Sound.h"

namespace tank
{

namespace
{
void drawTerrain(byte type, ushort x, ushort y)
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
}

bool Game::TryToFixPos(Point& newPos, const Point& prevPos) const
{
  if (newPos.x != prevPos.x)
  {
    // try to move down a pixel
    TileType tile1 = terrain.GetTile(newPos.x / SPRITE_SIZE, (newPos.y + 1) / SPRITE_SIZE);
    TileType tile2 = terrain.GetTile(newPos.x / SPRITE_SIZE, (newPos.y + SPRITE_SIZE) / SPRITE_SIZE);
    TileType tile3 = terrain.GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + 1) / SPRITE_SIZE);
    TileType tile4 = terrain.GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.y++;
      return true;
    }
    // try to move up a pixel
    tile1 = terrain.GetTile(newPos.x / SPRITE_SIZE, (newPos.y - 1) / SPRITE_SIZE);
    tile2 = terrain.GetTile(newPos.x / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 2) / SPRITE_SIZE);
    tile3 = terrain.GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y - 1) / SPRITE_SIZE);
    tile4 = terrain.GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 2) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.y--;
      return true;
    }
  }
  if (newPos.y != prevPos.y)
  {
    // try to move right a pixel
    TileType tile1 = terrain.GetTile((newPos.x + 1) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    TileType tile2 = terrain.GetTile((newPos.x + 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    TileType tile3 = terrain.GetTile((newPos.x + SPRITE_SIZE) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    TileType tile4 = terrain.GetTile((newPos.x + SPRITE_SIZE) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.x++;
      return true;
    }
    // try to move left a pixel
    tile1 = terrain.GetTile((newPos.x - 1) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    tile2 = terrain.GetTile((newPos.x - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    tile3 = terrain.GetTile((newPos.x + SPRITE_SIZE - 2) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
    tile4 = terrain.GetTile((newPos.x + SPRITE_SIZE - 2) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
    if ((tile1 == NONE) && (tile2 == NONE) && (tile3 == NONE) && (tile4 == NONE))
    {
      newPos.x--;
      return true;
    }
  }
  return false;
}

void Game::UpdateTankPosPlayer(Tank& tank)
{
  UpdateTankPos(tank, Input::GetPlayerDirection(tank.GetType()));
}

void Game::UpdateTankPosAI(Tank& tank)
{
  const Point prevPos = tank.GetPos();
  UpdateTankPos(tank, tank.GetDirection());
  const Point newPos = tank.GetPos();
  if (newPos.x == prevPos.x && newPos.y == prevPos.y)
  {
    tank.SetDirection((Direction)((rand() % 4) + 1));
  }
}

void Game::UpdateTankPos(Tank& tank, Direction dir)
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
  TileType tile1 = terrain.GetTile(newPos.x / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
  TileType tile2 = terrain.GetTile(newPos.x / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
  TileType tile3 = terrain.GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, newPos.y / SPRITE_SIZE);
  TileType tile4 = terrain.GetTile((newPos.x + SPRITE_SIZE - 1) / SPRITE_SIZE, (newPos.y + SPRITE_SIZE - 1) / SPRITE_SIZE);
  if ((tile1 != NONE) || (tile2 != NONE) || (tile3 != NONE) || (tile4 != NONE))
  {
    if (!TryToFixPos(newPos, prevPos))
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

void Game::UpdateTankFire(Tank& tank)
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

bool Game::UpdateBullet(Bullet& bullet)
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
  TileType tile = terrain.GetTile(bullet.pos.x / SPRITE_SIZE, bullet.pos.y / SPRITE_SIZE);
  if (tile == ARMOUR)
  {
    bullet.state = 0;
    return false;
  }
  if (tile == BRICK || tile == HQ)
  {
    terrain.ClearTile(bullet.pos.x / SPRITE_SIZE, bullet.pos.y / SPRITE_SIZE);
    bullet.pos.x = bullet.pos.x / SPRITE_SIZE * SPRITE_SIZE;
    bullet.pos.y = bullet.pos.y / SPRITE_SIZE * SPRITE_SIZE;
    bullet.state = EXPLOSION_FRAMES;
    Graphics::DrawExplosion(bullet.pos.x, bullet.pos.y, bullet.state);
    Sound::ExplosionSound();
    if (tile == HQ)
    {
      EndGame();
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

void Game::ClearBullet(Point pos) const
{
  TileType tile1 = terrain.GetTile(pos.x / SPRITE_SIZE, pos.y / SPRITE_SIZE);
  TileType tile2 = terrain.GetTile(pos.x / SPRITE_SIZE, (pos.y + BULLET_SIZE - 1) / SPRITE_SIZE);
  TileType tile3 = terrain.GetTile((pos.x + BULLET_SIZE - 1) / SPRITE_SIZE, pos.y / SPRITE_SIZE);
  TileType tile4 = terrain.GetTile((pos.x + BULLET_SIZE - 1) / SPRITE_SIZE, (pos.y + BULLET_SIZE - 1) / SPRITE_SIZE);
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

void Game::UpdateBullets()
{
  for (size_t i = 0; i < MAX_BULLETS; ++i)
  {
    Bullet& bullet = bullets[i];
    if (bullet.state == BULLET_STATE_FLYING)
    {
      // remove bullet at previous pos
      ClearBullet(bullet.pos);
      if (UpdateBullet(bullet))
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

void Game::DrawLevel() const
{
  Graphics::ClearScreen();
  for (int y = 0; y < TILE_COLUMNS; ++y)
  {
    for (int x = 0; x < TILE_ROWS; ++x)
    {
      if (byte type = terrain.GetTile(x, y))
      {
        drawTerrain(type, x * SPRITE_SIZE, y * SPRITE_SIZE);
      }
    }
  }
}

void Game::EndGame()
{
  gameOver = true;
  Graphics::DrawText(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 8, "Game");
  Graphics::DrawText(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2, "Over");
}

Game::Game()
{
  terrain.LoadLevel(1);
}

void Game::Setup()
{
  DrawLevel();
  tanks.Respawn();
  Sound::PlayIntroMusic();
}

void Game::Update()
{
  if (!gameOver)
  {
    const unsigned long timePassed = millis();
    if (timePassed >= lastStepTime + TANK_STEP_DURATION)
    {
      lastStepTime = timePassed;
      tanks.Respawn();
      UpdateTankPosPlayer(tanks.GetPlayer1());
      UpdateTankFire(tanks.GetPlayer1());
      UpdateTankPosPlayer(tanks.GetPlayer2());
      UpdateTankFire(tanks.GetPlayer2());
      UpdateTankPosAI(tanks.GetEnemy1());
      UpdateTankPosAI(tanks.GetEnemy2());
      UpdateTankFire(tanks.GetEnemy1());
      UpdateTankFire(tanks.GetEnemy2());
    }
    if (timePassed >= lastBulletsTime + BULLET_STEP_DURATION)
    {
      lastBulletsTime = timePassed;
      UpdateBullets();
    }
    if (tanks.PlayersAreDead())
    {
      EndGame();
    }
  }
}

}
