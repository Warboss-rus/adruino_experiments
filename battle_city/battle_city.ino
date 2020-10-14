#include "Display.h"
#include "Input.h"
#include "Sound.h"
#include "TankGame.h"

tank::Game tankGame;

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  Input::Setup();
  Display::Init();
  Sound::Setup();

  tankGame.Setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  tankGame.Update();
  Sound::Update();
}
