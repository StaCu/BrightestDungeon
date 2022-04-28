#pragma once

#include "Entity.h"

class __attribute__ ((packed)) LongEntity : public Entity {

protected:
  int16_t length;

public:

  void spawn(uint8_t mask, uint8_t pos, uint8_t length);
  void spawn(uint8_t mask, int16_t pos, int16_t length);

  int16_t getSize() const;

};
