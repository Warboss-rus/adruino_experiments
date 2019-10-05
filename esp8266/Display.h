#pragma once
#include <Arduino.h>

class Display
{
  public:
    static void init();
    static void setString(int idx, String str);
};
