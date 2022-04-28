#pragma once

#include "../LongEntity.h"

class __attribute__ ((packed)) Water : public LongEntity {

private:
  int8_t current_offset;
  int8_t current_step;
  int8_t current_speed;

public:
  static Water *spawn(uint8_t *args);

  void update();

  void collision(Entity &entity, int16_t depth);

  void draw();

  int8_t getCurrent();

};
