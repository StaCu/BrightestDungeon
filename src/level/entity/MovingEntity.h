#pragma once

#include "Entity.h"

class __attribute__ ((packed)) MovingEntity : public Entity {

private:
  int16_t old_pos;

protected:
  int8_t sub_step;

public:

  void spawn(uint8_t mask, int16_t pos);

  void update();

  int8_t move(int16_t steps, bool edges = true);

  void jump(int16_t jump_pos);

  int16_t getOldPosition();

};
