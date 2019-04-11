#pragma once
#include "Common.h"

class Graphics
{
  public:
    static void InitScreen();
    static void ClearScreen();
    static void ClearSprite(byte x, byte y);
    static void ClearBullet(byte x, byte y);
    static void DrawBricks(byte x, byte y);
    static void DrawArmour(byte x, byte y);
    static void DrawTrees(byte x, byte y);
    static void DrawWater(byte x, byte y);
    static void DrawHQ(byte x, byte y);
    static void DrawBullet(byte x, byte y);
    static void DrawTank(byte x, byte y, Direction dir, TankIndex index);
    static void DrawExplosion(byte x, byte y, byte frame);
};
