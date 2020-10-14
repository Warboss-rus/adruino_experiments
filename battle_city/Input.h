#include "TankCommon.h"

class Input
{
  public:
    static void Setup();
    static tank::Direction GetPlayerDirection(tank::TankIndex player);
    static bool GetFireButtonPressed(tank::TankIndex player);
};
