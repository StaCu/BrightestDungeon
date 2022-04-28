#include "Portal.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

Portal *Portal::spawn_one(uint8_t mask, int16_t pos0, int16_t pos1, bool inverted) {
    Portal *portal = (Portal*) Pool::allocate(PORTAL_ID);
    if (portal) {
        portal->Entity::spawn(mask, pos0);
        portal->exit = pos1;
        portal->inverted = inverted;
    }
    return portal;
}

Portal *Portal::spawn(uint8_t *args) {
    return Portal::spawn(args[0], Options::scalePosition(args[1]), Options::scalePosition(args[2]));
}

Portal *Portal::spawn(uint8_t mask, int16_t pos0, int16_t pos1) {
    bool inverted = pos0 < pos1;
    Portal::spawn_one(mask, pos0, pos1, inverted);
    return Portal::spawn_one(mask, pos1, pos0, inverted);
}

void Portal::update() {

}

void Portal::collision(Entity &entity, int16_t depth) {}

void Portal::draw() {
    uint8_t color_begin, color_end;
    if ((exit > getPosition()) == inverted) {
        color_begin = COLOR_PORTAL_BLUE_BEGIN;
        color_end   = COLOR_PORTAL_BLUE_END;
    } else {
        color_begin = COLOR_PORTAL_ORANGE_BEGIN;
        color_end   = COLOR_PORTAL_ORANGE_END;
    }
    
    // Draw the outside pixels slightly brighter than the inside
    Panel::draw(getPosition(), color_begin);
    Panel::drawLine(getPosition()+1, getSize()-2, color_end);
    Panel::draw(getPosition()+getSize()-1, color_begin);
}

int16_t Portal::getSize() const {
    return Options::size_medium;
}

int16_t Portal::getExit() const {
    return exit;
}

bool Portal::isInverted() const {
    return inverted;
}
