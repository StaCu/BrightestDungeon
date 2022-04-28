#include "Controller.h"

#include "../../level/Options.h"

bool Controller::attack = 0;
int8_t Controller::angle = 0;

bool Controller::poll() {
  bool input = false;
  int8_t angle = 0;
  bool attack = false;
  poll(angle, attack);

  if (Options::controler_direction) {
    angle = 0 - angle;
  }
  if (angle != 0) {
    input = true;
    Controller::angle = angle * abs(angle) / 100 + (angle > 0 ? 1 : -1);
  } else {
    Controller::angle = 0;
  }

  Controller::attack = attack;
  if (attack) {
    input = true;
  }

#if LOG_CONTROLLER
    LOG("Controller(a: ");
    LOG(getAngle());
    LOG(", w: ");
    LOG(""+isAttack());
    LOG_LN(")");
#endif

  return input;
}

int8_t Controller::getAngle() {
  return angle;
}

bool Controller::isAttack() {
  return attack;
}
