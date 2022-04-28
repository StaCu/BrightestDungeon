#pragma once

#include "../MovingEntity.h"

class Sword;
class Shot;
class ForceField;
class Lava;
class Wall;
class Block;

class __attribute__ ((packed)) Monster : public MovingEntity {

private:
  int8_t speed;

public:
  static Monster *spawn(uint8_t *args);
  static Monster *spawn(uint8_t mask, int16_t pos, int8_t speed);

  void update();

  void collision(Entity &entity, int16_t depth);
  
  void collision(Sword &sword, int16_t depth);

  void collision(Shot &shot, int16_t depth);

  void collision(Block &block, int16_t depth);

  void collision(ForceField &forcefield, int16_t depth);

  void collision(Lava &lava, int16_t depth);

  void collision(Wall &wall, int16_t depth);

  void draw();

  int16_t getSize();

};
