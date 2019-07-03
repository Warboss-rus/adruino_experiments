#pragma once
#include "Common.h"

class Graphics
{
  public:
    static void InitScreen();
    static void ClearScreen();
    static void ClearSprite(Point pos);
    static void ClearMovingSprite(Point oldPos, Point newPos);
    static void ClearBullet(Point pos);
    static void DrawBricks(byte x, byte y);
    static void DrawArmour(byte x, byte y);
    static void DrawTrees(byte x, byte y);
    static void DrawWater(byte x, byte y);
    static void DrawHQ(byte x, byte y);
    static void DrawBullet(Point pos);
    static void DrawTank(Point pos, Direction dir, TankIndex index);
    static void DrawExplosion(byte x, byte y, byte frame);
    static void DrawText(byte x, byte y, const char* text);
  private:
    static void DrawSprite(byte x, byte y, const byte* sprite, uint16_t frontColor, uint16_t backColor);
    static void DrawTransparentSprite(byte x, byte y, const byte* sprite, uint16_t color);
};
