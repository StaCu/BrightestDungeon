#include "Lava.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"
#include "../../../util/Random.h"

Lava *Lava::spawn(uint8_t *args) {
    Lava *lava = (Lava*) Pool::allocate(LAVA_ID);
    if (lava) {
        lava->LongEntity::spawn(args[0], args[1], args[2]);
        lava->timer = Timer(args[3], args[4]);
        lava->on = false;
    }
    return lava;
}

void Lava::update() {
  if (timer.dec()) {
    on = !on;
  }
}

void Lava::collision(Entity &entity, int8_t depth) {}

void Lava::draw() {
  if (isOn()) {
    Panel::drawLine(getPosition(), length, Random::get(4) ? COLOR_LAVA_ON : COLOR_LAVA_OFF);
  } else {
    Panel::drawLine(getPosition(), length, COLOR_LAVA_OFF);
  }
}

bool Lava::isOn() {
  return on;
}
