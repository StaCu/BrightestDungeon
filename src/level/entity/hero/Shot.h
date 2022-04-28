#pragma once

#include "../MovingEntity.h"

class ForceField;
class Lava;

class __attribute__ ((packed)) Shot : public MovingEntity {

private:
  static uint8_t active;

  int8_t speed;
  uint8_t active_mask;

public:
  static Shot *spawn(int16_t pos, int8_t speed);

  void destroy();

  void update();

  static void reset();

  void collision(Entity &entity, int8_t depth);

  void collision(ForceField &forcefield, int16_t depth);

  void collision(Lava &lava, int8_t depth);

  void draw();

  int16_t getSize();

};
