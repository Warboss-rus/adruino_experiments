#include "Common.h"

class Tank
{
  public:
    constexpr Tank(byte x = 0, byte y = 0, Direction dir = DIR_UP, TankIndex type = TANK_P1, bool alive = false)
      : m_pos(Point{x, y}), m_dir(dir), m_type(type), m_alive(alive)
    {}
    Tank(const Tank& other) = default;
    Tank& operator=(const Tank& other) = default;

    Point GetPos() const
    {
      return m_pos;
    }

    Direction GetDirection() const
    {
      return m_dir;
    }

    TankIndex GetType() const
    {
      return m_type;
    }

    bool IsAlive() const
    {
      return m_alive;
    }

    bool IsReloading() const
    {
      return m_reload > 0;
    }

    void SetAlive(bool alive)
    {
      m_alive = alive;
    }

    void Reload()
    {
      --m_reload;
    }

    void StartReload()
    {
      m_reload = RELOAD_FRAMES;
    }

    void SetDirection(Direction dir)
    {
      m_dir = dir;
    }

    void SetPosition(Point pos)
    {
      m_pos = pos;
    }

    void Kill()
    {
      m_alive = false;
      m_reload = RESPAWN_DELAY_FRAMES;
    }

    bool CanRespawn()
    {
      if (!m_alive && m_reload > 0)
      {
        --m_reload;
      }
      return m_reload == 0;
    }

  private:
    Point m_pos = {};
    Direction m_dir = DIR_UP;
    TankIndex m_type = TANK_P1;
    byte m_reload = 0;
    bool m_alive = false;
};

class TankManager
{
  public:
    void Respawn();

    Tank& GetPlayer1()
    {
      return m_player1;
    }

    Tank& GetPlayer2()
    {
      return m_player2;
    }

    Tank& GetEnemy1()
    {
      return m_enemy1;
    }

    Tank& GetEnemy2()
    {
      return m_enemy2;
    }

    bool CollidesWithTanks(const Tank& tank) const;
    Tank* CollidesWithTanks(const Bullet& bullet);

    bool PlayersAreDead() const
    {
      return m_player1Lives == 0 && m_player2Lives == 0 && !m_player1.IsAlive() && !m_player2.IsAlive();
    }

  private:
    bool TankPosFree(const Tank& tank);
    Tank* CollidesWithTanks(Point pos, byte width, byte height, TankIndex type);

    Tank m_player1;
    Tank m_player2;
    Tank m_enemy1;
    Tank m_enemy2;

    byte m_player1Lives = PLAYER_LIVES;
    byte m_player2Lives = PLAYER_LIVES;
    byte m_enemyLives = ENEMY_LIVES;
};
