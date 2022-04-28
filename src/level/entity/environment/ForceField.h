#pragma once

#include "../Entity.h"

class __attribute__ ((packed)) ForceField : public Entity {

private:
  bool color;

public:
  static ForceField *spawn(uint8_t *args);

  void update();

  void collision(Entity &entity, int8_t depth);

  void draw();

  int16_t getSize();

  bool getColor();

};
