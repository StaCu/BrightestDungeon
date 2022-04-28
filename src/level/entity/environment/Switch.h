#pragma once

#include "../Entity.h"

class __attribute__ ((packed)) Switch : public Entity {

private:
  uint8_t operation;
  bool used;

public:

  static Switch *spawn(uint8_t *args);
  static Switch *spawn(uint8_t mask, int16_t pos, uint8_t operation);

  void update();

  void collision(Entity &entity, int8_t depth);

  void draw();

  int16_t getSize();

  void use();

};
