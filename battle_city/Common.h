#pragma once
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define SPRITE_SIZE 8
#define BULLET_SIZE 2
#define TILE_ROWS 16 // SCREEN_WIDTH / SPRITE_SIZE
#define TILE_COLUMNS 20 // SCREEN_HEIGHT / SPRITE_SIZE
#define TANK_STEP_DURATION 50
#define BULLET_STEP_DURATION 20
#define RELOAD_FRAMES 40
#define MAX_BULLETS 10
#define BULLET_STATE_FLYING 255
#define EXPLOSION_FRAMES 5
#define PLAYER_LIVES 3
#define ENEMY_LIVES 20
#define RESPAWN_DELAY_FRAMES 60

using byte = unsigned char;

enum Direction: byte
{
  DIR_NONE,
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
};

enum TankIndex: byte
{
  TANK_ENEMY,
  TANK_P1,
  TANK_P2,
  WORLD
};

struct Point
{
  byte x;
  byte y;
};

struct Bullet
{
  Point pos;
  Direction dir;
  TankIndex owner;
  byte state; // 0 - disabled, 1-EXPLOSION_FRAMES - explosion, 255 - flying
};
