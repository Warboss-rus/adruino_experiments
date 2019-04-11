#pragma once
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define SPRITE_SIZE 8
#define BULLET_SIZE 2
#define TILE_ROWS 16 // SCREEN_WIDTH / SPRITE_SIZE
#define TILE_COLUMNS 20 // SCREEN_HEIGHT / SPRITE_SIZE

enum Direction
{
  DIR_NONE,
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
};

enum TankIndex
{
  TANK_ENEMY,
  TANK_P1,
  TANK_P2
};
