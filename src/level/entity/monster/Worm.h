#pragma once

#include "../MovingEntity.h"

class Sword;
class Shot;
class ForceField;
class Lava;
class Water;
class Wall;

class __attribute__ ((packed)) Worm : public MovingEntity {

private:

  int8_t cut;
  uint8_t contracting_and_length;
  int8_t speed;

  void collision_turn(int16_t depth);

  void collision_harm(int16_t depth);

public:
  static Worm *spawn(uint8_t *args);
  static Worm *spawn(uint8_t mask, int16_t pos, int8_t speed);

  void update();

  void collision(Entity &entity, int16_t depth);

  void collision(Sword &sword, int16_t depth);

  void collision(Shot &shot, int16_t depth);

  void collision(ForceField &forcefield, int16_t depth);

  void collision(Lava &lava, int16_t depth);

  void collision(Water &water, int16_t depth);

  void collision(Wall &wall, int16_t depth);

  void draw();

  int16_t getSize();

};
