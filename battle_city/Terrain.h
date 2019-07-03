#pragma once

enum TileType
{
  NONE = 0,
  BRICK = 1,
  HQ = 2,
  ARMOUR = 3,
  TREE = 4,
  WATER = 5,
};

using byte = unsigned char;

class Terrain
{
  public:
    static TileType GetTile(byte x, byte y);
    static void ClearTile(byte x, byte y);
};
