#define MAX_LENGTH 255
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define INITIAL_STEP_DURATION 125
#define SNAKE_GROW_PER_APPLE 3
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_DEADZONE_MIN 256
#define JOYSTICK_DEADZONE_MAX 768

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
  Serial.write(startMarker);
  Serial.write(actionClear);
  Serial.write(SCREEN_WIDTH);
  Serial.write(SCREEN_HEIGHT);
}

void setPixel(unsigned char x, unsigned char y, bool lit)
{
  Serial.write(startMarker);
  Serial.write(actionSetPixel);
  Serial.write(x);
  Serial.write(y);
  Serial.write(lit ? 1 : 0);
}

void updateDirection()
{
  const int x = analogRead(JOYSTICK_X_PIN);
  const int y = analogRead(JOYSTICK_Y_PIN);
  const int joystickNeutral = (JOYSTICK_DEADZONE_MIN + JOYSTICK_DEADZONE_MAX) / 2;
  const int deltaX = abs(x - joystickNeutral);
  const int deltaY = abs(y - joystickNeutral);
  if (deltaX > deltaY)
  {
    if (x <= JOYSTICK_DEADZONE_MIN && prevSnakeDirection != RIGHT)
    {
      snakeDirection = LEFT;
    }
    else if (x >= JOYSTICK_DEADZONE_MAX && prevSnakeDirection != LEFT)
    {
      snakeDirection = RIGHT;
    }
  }
  else
  {
     if (y <= JOYSTICK_DEADZONE_MIN && prevSnakeDirection != DOWN)
    {
      snakeDirection = UP;
    }
    else if (y >= JOYSTICK_DEADZONE_MAX && prevSnakeDirection != UP)
    {
      snakeDirection = DOWN;
    }
  }
  // TODO: do not allow 180 turns
}

void gameOver()
{
  snakeDirection = NONE;
  gameIsOver = true;
  // TODO: display game over message
}

inline unsigned char nextIndex(unsigned char i)
{
  return (i == MAX_LENGTH) ? 0 : (i + 1);
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
  Point headPos = snakeBody[endIdx == 0 ? MAX_LENGTH : endIdx - 1];
  prevSnakeDirection = snakeDirection;
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
  if (headPos.x == applePos.x && headPos.y == applePos.y)
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
  randomSeed(analogRead(0));
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  Serial.begin(9600);
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
  }
}
