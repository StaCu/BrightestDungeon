#include "Shot.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"

#include "../environment/ForceField.h"
#include "../environment/Lava.h"

uint8_t Shot::active;

Shot *Shot::spawn(int16_t pos, int8_t speed) {
    uint8_t active_mask = 0b1;
    if (speed > 0) active_mask = 0b10;
    if (active & active_mask) return nullptr;
    Shot *shot = (Shot*) Pool::allocate(SHOT_ID, true);
    if (shot) {
        shot->MovingEntity::spawn(0, pos);
        shot->speed = speed;
        shot->active_mask = active_mask;
        active |= active_mask;

        Sound::play(Sound::ATTACK);
    }
    return shot;
}

void Shot::destroy() {
  active &= ~active_mask;
  Entity::destroy();
}

void Shot::update() {
  MovingEntity::update();
  int8_t side = move(speed);
  if (side) {
    // shot left the level
    this->destroy();
  }
}

void Shot::reset() {
  active = 0;
}

void Shot::collision(Entity &entity, int8_t depth) {
  this->destroy();
}
void Shot::collision(ForceField &forcefield, int16_t depth) {
  this->destroy();
}
void Shot::collision(Lava &lava, int8_t depth) {
  if (lava.isOn()) {
    this->destroy();
  }
}

void Shot::draw() {
  Panel::draw(getPosition(), COLOR_ATTACK);
}

int16_t Shot::getSize() {
  return 1;
}
