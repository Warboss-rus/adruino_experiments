#define MAX_LENGTH 250
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define INITIAL_STEP_DURATION 125
#define SNAKE_GROW_PER_APPLE 5
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_DEADZONE_MIN 256
#define JOYSTICK_DEADZONE_MAX 768
#define JOYSTICK_NEUTRAL 512
//#define ENABLE_SERIAL_DISPLAY
#define ENABLE_PHYSICAL_DISPLAY

#ifdef ENABLE_PHYSICAL_DISPLAY
#include <U8g2lib.h>
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, 10);
#endif

struct Point
{
  unsigned char x;
  unsigned char y;
};
enum Direction
{
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

Point snakeBody[MAX_LENGTH];
unsigned char startIdx = 0;
unsigned char endIdx = 0;
Point applePos = {};
Direction snakeDirection = NONE;
Direction prevSnakeDirection = RIGHT;
int stepDuration = INITIAL_STEP_DURATION;
unsigned long lastStepTime = 0;
unsigned int score = 0;
unsigned char snakeGrow = 0;
bool gameIsOver = false;
const byte startMarker = 0x4E;
const byte actionSetPixel = 1;
const byte actionClear = 2;

void clearScreen()
{
#ifdef ENABLE_SERIAL_DISPLAY
  Serial.write(startMarker);
  Serial.write(actionClear);
  Serial.write(SCREEN_WIDTH);
  Serial.write(SCREEN_HEIGHT);
#endif
#ifdef ENABLE_PHYSICAL_DISPLAY
  u8g2.clearBuffer();
#endif
}

void setPixel(unsigned char x, unsigned char y, bool lit)
{
#ifdef ENABLE_SERIAL_DISPLAY
  Serial.write(startMarker);
  Serial.write(actionSetPixel);
  Serial.write(x);
  Serial.write(y);
  Serial.write(lit ? 1 : 0);
#endif
#ifdef ENABLE_PHYSICAL_DISPLAY
  u8g2.setDrawColor(lit ? 1 : 0);
  u8g2.drawPixel(x, y);
#endif
}

void updateDirection()
{
  const int x = analogRead(JOYSTICK_X_PIN);
  const int y = analogRead(JOYSTICK_Y_PIN);
  const int deltaX = abs(x - JOYSTICK_NEUTRAL);
  const int deltaY = abs(y - JOYSTICK_NEUTRAL);
  if (deltaX > deltaY)
  {
    if ((x <= JOYSTICK_DEADZONE_MIN) && (prevSnakeDirection != RIGHT))
    {
      snakeDirection = LEFT;
    }
    else if ((x >= JOYSTICK_DEADZONE_MAX) && (prevSnakeDirection != LEFT))
    {
      snakeDirection = RIGHT;
    }
  }
  else
  {
     if ((y <= JOYSTICK_DEADZONE_MIN) && (prevSnakeDirection != DOWN))
    {
      snakeDirection = UP;
    }
    else if ((y >= JOYSTICK_DEADZONE_MAX) && (prevSnakeDirection != UP))
    {
      snakeDirection = DOWN;
    }
  }
}

void gameOver()
{
  snakeDirection = NONE;
  gameIsOver = true;
  // TODO: display game over message
}

void endOfLevel()
{
    stepDuration = stepDuration * 4 / 5; //increase snake speed
    clearScreen();
    resetSnake();
    spawnApple();
}

inline unsigned char nextIndex(unsigned char i)
{
  return (i + 1 == MAX_LENGTH) ? 0 : (i + 1);
}

void spawnApple() 
{
  while(true)
  {
    applePos.x = random(SCREEN_WIDTH);
    applePos.y = random(SCREEN_HEIGHT);
    // if apple is inside the snake, try another random point
    for (unsigned char i = startIdx; i != endIdx; i = nextIndex(i))
    {
      if (applePos.x == snakeBody[i].x && applePos.y == snakeBody[i].y)
      {
        continue;
      }
    }
    break;
  }
  setPixel(applePos.x, applePos.y, true);
}

void resetSnake() 
{
  startIdx = 0;
  endIdx = 3;
  snakeDirection = NONE;
  prevSnakeDirection = RIGHT;
  snakeGrow = 0;
  const int centerx = SCREEN_WIDTH / 2;
  const int centery = SCREEN_HEIGHT / 2;
  snakeBody[0].x = centerx - 1;
  snakeBody[0].y = centery;
  setPixel(snakeBody[0].x, snakeBody[0].y, true);
  snakeBody[1].x = centerx;
  snakeBody[1].y = centery;
  setPixel(snakeBody[1].x, snakeBody[1].y, true);
  snakeBody[2].x = centerx + 1;
  snakeBody[2].y = centery;
  setPixel(snakeBody[2].x, snakeBody[2].y, true);
}

void nextStep() 
{
  Point headPos = snakeBody[endIdx == 0 ? MAX_LENGTH - 1 : endIdx - 1];
  switch(snakeDirection)
  {
    case UP:
      headPos.y = (headPos.y == 0 ? SCREEN_HEIGHT : headPos.y) - 1;
      break;
    case DOWN:
      headPos.y = (headPos.y + 1) % SCREEN_HEIGHT;
      break;
    case LEFT:
      headPos.x = (headPos.x == 0 ? SCREEN_WIDTH : headPos.x) - 1;
      break;
    case RIGHT:
      headPos.x = (headPos.x + 1) % SCREEN_WIDTH;
      break;
    default:
      return;
  }
  prevSnakeDirection = snakeDirection;
  // check if we hit our body
  for (unsigned char i = startIdx; i != endIdx; i = nextIndex(i))
  {
    if (headPos.x == snakeBody[i].x && headPos.y == snakeBody[i].y)
    {
      gameOver();
      return;
    }
  }
  snakeBody[endIdx] = headPos;
  endIdx = nextIndex(endIdx);
  if (endIdx == startIdx)
  {
    endOfLevel();
  }
  else if (headPos.x == applePos.x && headPos.y == applePos.y)
  {
    //spawn new apple, do not move tail, do not switch head since this pixel is already on because of an apple
    spawnApple();
    ++score;
    snakeGrow += SNAKE_GROW_PER_APPLE - 1;
  }
  else
  {
    // move head and tail
    setPixel(headPos.x, headPos.y, true);
    if (snakeGrow == 0)
    {
      setPixel(snakeBody[startIdx].x, snakeBody[startIdx].y, false);
      startIdx = nextIndex(startIdx);
    }
    else
    {
      --snakeGrow;
    }
  }
}

void setup() 
{
  // put your setup code here, to run once:
  randomSeed(analogRead(2));
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
#ifdef ENABLE_SERIAL_DISPLAY
  Serial.begin(9600);
#endif
#ifdef ENABLE_PHYSICAL_DISPLAY
  u8g2.begin();
#endif
  clearScreen();
  resetSnake();
  spawnApple();
}

void loop() 
{
  if (gameIsOver)
  {
    return;
  }
  // put your main code here, to run repeatedly:
  updateDirection();
  const unsigned long timePassed = millis();
  if (timePassed >= lastStepTime + stepDuration)
  {
    lastStepTime = timePassed;
    nextStep();
#ifdef ENABLE_PHYSICAL_DISPLAY
    u8g2.sendBuffer();
#endif
  }
}
