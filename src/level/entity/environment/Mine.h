#pragma once

#include "../Entity.h"

class Hero;
class Sword;
class Shot;
class Monster;
class Worm;
class Item;

class __attribute__ ((packed)) Mine : public Entity {

private:
  int16_t blast;

public:
  static Mine *spawn(uint8_t *args);
  static Mine *spawn(uint8_t mask, int16_t pos);

  void update();

  void collision(Entity &entity, int8_t depth);

private:
  void __attribute__((noinline)) collision(Entity &entity);

public:
  void collision(Sword &entity, int8_t depth);

  void collision(Shot &entity, int8_t depth);

  void collision(Hero &entity, int8_t depth);

  void collision(Monster &entity, int8_t depth);

  void collision(Worm &entity, int8_t depth);

  void draw();

  int16_t getSize();

};
