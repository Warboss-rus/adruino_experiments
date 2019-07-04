#include <Arduino.h>
#include "Tank.h"
#include "Graphics.h"

namespace
{
constexpr Tank PLAYER1_DEFAULT = Tank(40, 152, DIR_UP, TANK_P1);
constexpr Tank PLAYER2_DEFAULT = Tank(72, 152, DIR_UP, TANK_P2);
constexpr Tank ENEMY1_DEFAULT = Tank(0, 0, DIR_RIGHT, TANK_ENEMY);
constexpr Tank ENEMY2_DEFAULT = Tank(120, 0, DIR_DOWN, TANK_ENEMY);

bool CollidesWithTank(Point pos, byte width, byte height, const Tank& tank, TankIndex type)
{
  const Point tankPos = tank.GetPos();
  return tank.IsAlive() && (tank.GetType() != type) && (pos.x + width >= tankPos.x) && (pos.x <= tankPos.x + SPRITE_SIZE) && (pos.y + height >= tankPos.y) && (pos.y <= tankPos.y + SPRITE_SIZE);
}

bool CollidesWithTank(const Tank& self, const Tank& tank)
{
  const Point pos = self.GetPos();
  const Point tankPos = tank.GetPos();
  return tank.IsAlive() && (&self != &tank) && (pos.x + SPRITE_SIZE >= tankPos.x) && (pos.x <= tankPos.x + SPRITE_SIZE) && (pos.y + SPRITE_SIZE >= tankPos.y) && (pos.y <= tankPos.y + SPRITE_SIZE);
}
}

void TankManager::Respawn()
{
  if (!m_player1.IsAlive() && (m_player1Lives > 0) && m_player1.CanRespawn() && !CollidesWithTanks(PLAYER1_DEFAULT))
  {
    --m_player1Lives;
    m_player1 = PLAYER1_DEFAULT;
    m_player1.SetAlive(true);
    Graphics::DrawTank(m_player1.GetPos(), m_player1.GetDirection(), m_player1.GetType());
  }
  if (!m_player2.IsAlive() && (m_player2Lives > 0) && m_player2.CanRespawn() && !CollidesWithTanks(PLAYER2_DEFAULT))
  {
    --m_player2Lives;
    m_player2 = PLAYER2_DEFAULT;
    m_player2.SetAlive(true);
    Graphics::DrawTank(m_player2.GetPos(), m_player2.GetDirection(), m_player2.GetType());
  }
  if (!m_enemy1.IsAlive() && (m_enemyLives > 0) && m_enemy1.CanRespawn() && !CollidesWithTanks(ENEMY1_DEFAULT))
  {
    --m_enemyLives;
    m_enemy1 = ENEMY1_DEFAULT;
    m_enemy1.SetAlive(true);
    Graphics::DrawTank(m_enemy1.GetPos(), m_enemy1.GetDirection(), m_enemy1.GetType());
  }
  if (!m_enemy2.IsAlive() && (m_enemyLives > 0) && m_enemy2.CanRespawn() && !CollidesWithTanks(ENEMY2_DEFAULT))
  {
    --m_enemyLives;
    m_enemy2 = ENEMY2_DEFAULT;
    m_enemy2.SetAlive(true);
    Graphics::DrawTank(m_enemy2.GetPos(), m_enemy2.GetDirection(), m_enemy2.GetType());
  }
}

Tank* TankManager::CollidesWithTanks(Point pos, byte width, byte height, TankIndex type)
{
  if (CollidesWithTank(pos, width, height, m_player1, type))
  {
    return &m_player1;
  }
  if (CollidesWithTank(pos, width, height, m_player2, type))
  {
    return &m_player2;
  }
  if (CollidesWithTank(pos, width, height, m_enemy1, type))
  {
    return &m_enemy1;
  }
  if (CollidesWithTank(pos, width, height, m_enemy2, type))
  {
    return &m_enemy2;
  }
  return NULL;
}

bool TankManager::CollidesWithTanks(const Tank& tank) const
{
  return CollidesWithTank(tank, m_player1) || CollidesWithTank(tank, m_player2) || CollidesWithTank(tank, m_enemy1) || CollidesWithTank(tank, m_enemy2);
}

Tank* TankManager::CollidesWithTanks(const Bullet& bullet)
{
  return CollidesWithTanks(bullet.pos, BULLET_SIZE, BULLET_SIZE, bullet.owner);
}
