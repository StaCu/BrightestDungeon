#include "Switch.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "../../entity/hero/Hero.h"

Switch *Switch::spawn(uint8_t *args) {
    return Switch::spawn(args[0], Options::scalePosition(args[1]), args[2]);
}

Switch *Switch::spawn(uint8_t mask, int16_t pos, uint8_t operation) {
    Switch *sw = (Switch*) Pool::allocate(SWITCH_ID);
    if (sw) {
        sw->Entity::spawn(mask, pos);
        sw->operation = operation;
        sw->used = false;
    }
    return sw;
}

void Switch::update() {

}

void Switch::collision(Entity &entity, int8_t depth) {}

void Switch::draw() {
    uint8_t color = COLOR_SWITCH_ON;
    uint8_t color2 = COLOR_SWITCH_OFF;
    int8_t cutoff = getSize()*2 / 3;
    if (cutoff == 0) {
        cutoff = 1;
    }
    if (used) {
        color = COLOR_SWITCH_OFF;
        color2 = COLOR_SWITCH_ON;
        cutoff = getSize() - cutoff;
    }
    Panel::drawLine(getPosition(), cutoff, color);
    Panel::drawLine(getPosition()+cutoff, getSize()-cutoff, color2);
}

int16_t Switch::getSize() {
  return Options::size_medium;
}

void Switch::use() {
    Sound::play(Sound::TELEPORT);
    Entity::global_mask ^= operation;
    used = !used;
}
