#pragma once

#include "../LongEntity.h"

class Sword;

class __attribute__ ((packed)) Wall : public LongEntity {

public:
  static Wall* spawn(uint8_t *args);

  void update();

  void collision(Entity &entity, int16_t depth);

  void draw();

};
