#pragma once
#include "TankCommon.h"

namespace tank
{

enum TileType
{
  NONE = 0,
  BRICK = 1,
  HQ = 2,
  ARMOUR = 3,
  TREE = 4,
  WATER = 5,
};

constexpr int TILES_COUNT = TILE_ROWS*TILE_COLUMNS;

class Level
{
  public:
    void LoadLevel(int number);
    TileType GetTile(ushort x, ushort y) const;
    void ClearTile(ushort x, ushort y);
    
  private:
    byte m_level[TILES_COUNT];
};

}
