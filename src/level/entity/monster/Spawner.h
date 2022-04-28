#pragma once

#include "../Entity.h"
#include "../../../peripheral/Timer.h"

class __attribute__ ((packed)) Spawner : public Entity {

private:
  uint8_t speed;
  Timer timer;

public:
  static Spawner *spawn(uint8_t *args);
  static Spawner *spawn(uint8_t mask, uint8_t first, uint8_t interval, int16_t pos, int8_t speed);

  void update();

};
