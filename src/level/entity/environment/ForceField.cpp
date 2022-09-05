#include "ForceField.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"
#include "../../../util/Random.h"

ForceField *ForceField::spawn(uint8_t *args) {
    ForceField *forcefield = (ForceField*) Pool::allocate(FORCEFIELD_ID);
    if (forcefield) {
        forcefield->Entity::spawn(args[0], Options::scalePosition(args[1]));
        forcefield->color = args[2];
    }
    return forcefield;
}

void ForceField::update() {
}

void ForceField::collision(Entity &entity, int8_t depth) {}

void ForceField::draw() {
  if (getColor() != Options::swap_color) {
    Panel::drawLine(getPosition(), getSize(), Random::get(8) ? COLOR_ITEM_COLOR_1 : COLOR_HERO_1);
  } else {
    Panel::drawLine(getPosition(), getSize(), Random::get(8) ? COLOR_ITEM_COLOR_0 : COLOR_HERO_0);
  }
}

int16_t ForceField::getSize() {
  return Options::size_medium;
}

bool ForceField::getColor() {
  return color;
}
