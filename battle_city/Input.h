#include "Common.h"

class Input
{
  public:
    static void Setup();
    static Direction GetPlayerDirection(TankIndex player);
    static bool GetFireButtonPressed(TankIndex player);
};
