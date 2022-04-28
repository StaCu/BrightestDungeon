#include "Sword.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "Hero.h"
#include "../environment/ForceField.h"
#include "../environment/Wall.h"

Sword *Sword::spawn() {
    Sword *sword = (Sword*) Pool::allocate(SWORD_ID, true);
    if (sword) {
        sword->Entity::spawn((uint8_t) 0, (int16_t) 0);
        Sound::play(Sound::ATTACK);
        sword->timeout = SWORD_ATTACK_TIMEOUT_ITERATIONS;  
        uint8_t max_size = Options::size_large + 4;
        sword->size = max_size;
        if (sword->size % 2 == 0) sword->size++;
        sword->offset = -(sword->size / 2);
    }
    return sword;
}

void Sword::collision(Entity &entity, int8_t depth) {}

void Sword::collision(int16_t cpos, int16_t csize, int16_t depth) {
  int16_t hero_pos = Hero::position;

  if (hero_pos >= cpos) {
    int16_t ff_end = cpos + csize;
    if (hero_pos < ff_end) {
      this->destroy();
      return;
    }
    size -= ff_end - getPosition();
    offset += ff_end - getPosition();
  } else {
    size -= depth;
  }

  if (size <= 0) {
    this->destroy();
  }
}

void Sword::collision(ForceField &forcefield, int16_t depth) {
    this->collision(forcefield.getPosition(), forcefield.getSize(), depth);
}

void Sword::collision(Wall &wall, int16_t depth) {
    this->collision(wall.getPosition(), wall.getSize(), depth);
}

void Sword::update() {
  timeout--;
  if (timeout == 0) {
    this->destroy();
  }
}

void Sword::draw() {
  Panel::drawLine(getPosition(), getSize(), COLOR_ATTACK);
}

int16_t Sword::getSize() {
  return size;
}

int16_t Sword::getPosition() {
    return Hero::position + offset;
}

int16_t Sword::getOldPosition() {
    return Hero::position + offset;
}
