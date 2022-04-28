#pragma once

#include "../LongEntity.h"
#include "../../../peripheral/Timer.h"

class __attribute__ ((packed)) Lava : public LongEntity {

private:
  Timer timer;

  bool on;

public:
  static Lava *spawn(uint8_t *args);

  void update();

  void collision(Entity &entity, int8_t depth);

  void draw();
  
  bool isOn();

};
