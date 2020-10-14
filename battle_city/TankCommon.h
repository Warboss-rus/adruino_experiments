#pragma once

namespace tank
{
  
using ushort = unsigned short;
using byte = unsigned char;
constexpr ushort SCREEN_WIDTH = 256;
constexpr ushort SCREEN_HEIGHT = 320;
constexpr ushort SPRITE_SIZE = 16;
constexpr ushort BULLET_SIZE = 4;
constexpr ushort TILE_ROWS = SCREEN_WIDTH / SPRITE_SIZE;
constexpr ushort TILE_COLUMNS = SCREEN_HEIGHT / SPRITE_SIZE;
constexpr ushort TANK_STEP_DURATION = 50;
constexpr ushort BULLET_STEP_DURATION = 20;
constexpr ushort RELOAD_FRAMES = 40;
constexpr ushort MAX_BULLETS = 10;
constexpr ushort BULLET_STATE_FLYING = 255;
constexpr ushort EXPLOSION_FRAMES = 5;
constexpr ushort PLAYER_LIVES = 3;
constexpr ushort ENEMY_LIVES = 20;
constexpr ushort RESPAWN_DELAY_FRAMES = 60;

enum Direction : byte
{
  DIR_NONE,
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
};

enum TankIndex : byte
{
  TANK_ENEMY,
  TANK_P1,
  TANK_P2,
  TANK_ENEMY_FAST,
  WORLD
};

struct Point
{
  ushort x;
  ushort y;
};

struct Bullet
{
  Point pos;
  Direction dir;
  TankIndex owner;
  byte state; // 0 - disabled, 1-EXPLOSION_FRAMES - explosion, 255 - flying
};

}
