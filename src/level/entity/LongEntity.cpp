#include "LongEntity.h"

#include "../Options.h"

void LongEntity::spawn(uint8_t mask, uint8_t pos, uint8_t length) {
  Entity::spawn(mask, Options::scalePosition(pos));
  this->length = Options::scalePosition(pos + length) - this->pos;
  if (this->length == 0) this->length = 1;
}

void LongEntity::spawn(uint8_t mask, int16_t pos, int16_t length) {
  Entity::spawn(mask, pos);
  this->length = length;
}

int16_t LongEntity::getSize() const {
    return length;
}
