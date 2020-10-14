#pragma once
#include "TankCommon.h"

namespace tank
{
class Graphics
{
  public:
    static void ClearScreen();
    static void ClearSprite(Point pos);
    static void ClearMovingSprite(Point oldPos, Point newPos);
    static void ClearBullet(Point pos);
    static void DrawBricks(ushort x, ushort y);
    static void DrawArmour(ushort x, ushort y);
    static void DrawTrees(ushort x, ushort y);
    static void DrawWater(ushort x, ushort y);
    static void DrawHQ(ushort x, ushort y);
    static void DrawBullet(Point pos);
    static void DrawTank(Point pos, Direction dir, TankIndex index);
    static void DrawExplosion(ushort x, ushort y, byte frame);
    static void DrawText(ushort x, ushort y, const char* text);
  private:
    static void DrawSprite(ushort x, ushort y, const byte* sprite, uint16_t frontColor, uint16_t backColor);
    static void DrawTransparentSprite(ushort x, ushort y, const byte* sprite, uint16_t color);
};
}
