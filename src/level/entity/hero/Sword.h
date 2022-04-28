#pragma once

#include "../Entity.h"

class ForceField;
class Wall;

class __attribute__ ((packed)) Sword : public Entity {

private:
  int8_t offset;
  int8_t size;
  uint8_t timeout;

  void collision(int16_t cpos, int16_t csize, int16_t depth);

public:

  static Sword *spawn();

  void collision(Entity &entity, int8_t depth);

  void collision(ForceField &forcefield, int16_t depth);

  void collision(Wall &wall, int16_t depth);

  void update();

  void draw();

  int16_t getSize();

  int16_t getPosition();

  int16_t getOldPosition();

};
