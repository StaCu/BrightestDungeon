#pragma once

#include "../Entity.h"
#include "../../../util/Timer.h"

class __attribute__ ((packed)) Button : public Entity {

private:
  uint8_t operation;
  Timer timer;

  bool used;

public:

  static Button *spawn(uint8_t *args);
  static Button *spawn(uint8_t mask, int16_t pos, uint8_t operation, uint8_t first, uint8_t interval);

  void update();
  
  void collision(Entity &entity, int8_t depth);

  void draw();

  int16_t getSize();

};
