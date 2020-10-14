#include "Input.h"
#include <Arduino.h>
#include "Pins.h"
#define JOYSTICK_DEADZONE_MIN 1024
#define JOYSTICK_DEADZONE_MAX 3072
#define JOYSTICK_NEUTRAL 2048

using namespace tank;

Direction getCurrentDirection(uint32_t xPin, uint32_t yPin)
{
  const int x = analogRead(xPin);
  const int y = analogRead(yPin);
  const int deltaX = abs(x - JOYSTICK_NEUTRAL);
  const int deltaY = abs(y - JOYSTICK_NEUTRAL);
  if (deltaX > deltaY)
  {
    if (x <= JOYSTICK_DEADZONE_MIN)
    {
      return DIR_LEFT;
    }
    else if (x >= JOYSTICK_DEADZONE_MAX)
    {
      return DIR_RIGHT;
    }
  }
  else
  {
    if (y <= JOYSTICK_DEADZONE_MIN)
    {
      return DIR_UP;
    }
    else if (y >= JOYSTICK_DEADZONE_MAX)
    {
      return DIR_DOWN;
    }
  }
  return DIR_NONE;
}

void Input::Setup()
{
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK2_BUTTON_PIN, INPUT_PULLUP);
  //pinMode(JOYSTICK_X_PIN, INPUT_ANALOG);
  //pinMode(JOYSTICK_Y_PIN, INPUT_ANALOG);
  //pinMode(JOYSTICK2_X_PIN, INPUT_ANALOG);
  //pinMode(JOYSTICK2_Y_PIN, INPUT_ANALOG);
}

Direction Input::GetPlayerDirection(TankIndex player)
{
  switch (player)
  {
    case TANK_P1:
      return getCurrentDirection(JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
    case TANK_P2:
      return getCurrentDirection(JOYSTICK2_X_PIN, JOYSTICK2_Y_PIN);
    default:
      return DIR_NONE;
  }
}

bool Input::GetFireButtonPressed(TankIndex player)
{
  switch (player)
  {
    case TANK_P1:
      return !digitalRead(JOYSTICK_BUTTON_PIN);
    case TANK_P2:
      return !digitalRead(JOYSTICK2_BUTTON_PIN);
    default:
      return false;
  }
}
