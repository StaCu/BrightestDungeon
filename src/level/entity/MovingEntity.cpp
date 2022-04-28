#include "MovingEntity.h"

#include "../Options.h"

void MovingEntity::spawn(uint8_t mask, int16_t pos) {
  Entity::spawn(mask, pos);
  this->old_pos = pos;
  this->sub_step = 0;
}

void MovingEntity::update() {
  old_pos = getPosition();
}

int8_t MovingEntity::move(int16_t steps, bool edges) {
  int16_t tmp = sub_step;
  tmp += steps;   

  if (tmp >= Options::sub_steps) {
    do {
      tmp -= Options::sub_steps;
      pos++;
    } while (tmp >= Options::sub_steps);
    if (pos > Options::level_length-1 && edges) {
      pos = Options::level_length-1;
      return 1;
    }
  } else if (tmp < 0) {
    do {
      tmp += Options::sub_steps;
      pos--;
    } while (tmp < 0);
    if (pos < 0 && edges) {
      pos = 0;
      return -1;
    }
  }
  sub_step = (int8_t) tmp;
  return 0;
}

void MovingEntity::jump(int16_t jump_pos) {
  Entity::jump(jump_pos);
  sub_step = 0;
  old_pos = jump_pos;
}

int16_t MovingEntity::getOldPosition() {
  return old_pos;
}
