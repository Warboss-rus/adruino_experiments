#pragma once
#include "TankLevel.h"
#include "TankManager.h"

namespace tank
{

class Game
{
  public:
    Game();
    void Setup();
    void Update();

  private:
    void UpdateBullets();
    bool UpdateBullet(Bullet& bullet);
    void UpdateTankPos(Tank& tank, Direction dir);
    void UpdateTankFire(Tank& tank);
    void UpdateTankPosAI(Tank& tank);
    void UpdateTankPosPlayer(Tank& tank);
    void EndGame();
    bool TryToFixPos(Point& newPos, const Point& prevPos) const;
    void DrawLevel() const;
    void ClearBullet(Point pos) const;

    unsigned long lastStepTime = 0;
    unsigned long lastBulletsTime = 0;
    TankManager tanks;
    Level terrain;
    Bullet bullets[MAX_BULLETS] = {};
    bool gameOver = false;
};

}
