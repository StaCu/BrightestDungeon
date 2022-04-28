#pragma once

#include "../../Settings.h"

class Controller {

private:
  static bool attack;
  static int8_t angle;

protected:
  static void poll(int8_t &angle, bool &attack);

public:
  static void init();
  
  static void callibrate();

  static bool poll();

  static int8_t getAngle();
  static bool isAttack();

};
