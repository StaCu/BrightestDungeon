#include "Fog.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

Fog *Fog::spawn(uint8_t *args) {
    Fog *fog = (Fog*) Pool::allocate(FOG_ID);
    if (fog) {
        fog->MovingEntity::spawn(args[0], Options::scalePosition(args[1]));
        fog->length = Options::scalePosition(args[1] + args[2]) - fog->pos;
        fog->speed  = args[3];
    }
    return fog;
}

void Fog::update() {
    MovingEntity::update();
    int16_t delta = move(speed, false);
    if (getPosition() + length < 0) {
        this->jump(Options::level_length);
    } else if (getPosition() >= (int16_t) Options::level_length) {
        this->jump(0 - length);
    }
}

void Fog::draw() {
    Panel::drawLine(getPosition(), getSize(), COLOR_FOG);
}

int16_t Fog::getSize() {
    return length;
}
