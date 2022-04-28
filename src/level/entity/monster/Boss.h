#pragma once

#include "../Entity.h"
#include "../../../peripheral/Timer.h"

class Sword;
class Shot;

class __attribute__ ((packed)) Boss : public Entity {

private:
  int8_t length;
  uint8_t next_pos;
  uint8_t last_pos;

  Timer timer;

  void collision(int16_t depth);

public:
  static Boss *spawn(uint8_t *args);
  static Boss *spawn(uint8_t mask, int16_t pos0, uint8_t pos1, uint8_t pos2);

  void update();

  void collision(Entity &entity, int16_t depth);

  void collision(Sword &sword, int16_t depth);

  void collision(Shot &shot, int16_t depth);

  void draw();

  int16_t getSize();

};
