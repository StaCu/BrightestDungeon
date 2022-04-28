#pragma once

#include "../Entity.h"

#define DOOR_ENTER_DELAY 1000

class __attribute__ ((packed)) Door : public Entity {

private:
  uint8_t linked_level;
  uint8_t dst;

public:
  static Door *spawn(uint8_t *args);
  static Door *spawn(uint8_t mask, int16_t pos, uint8_t linked_level, uint8_t dst);

  void update();

  void collision(Entity &entity, int8_t depth);

  void draw();

  int16_t getSize();

  uint8_t getDst();

  uint8_t getLinkedLevel();

  void enter();

};
