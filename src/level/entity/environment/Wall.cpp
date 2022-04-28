#include "Wall.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "../hero/Sword.h"

Wall *Wall::spawn(uint8_t *args) {
    Wall *wall = (Wall*) Pool::allocate(WALL_ID);
    if (wall) {
        wall->LongEntity::spawn(args[0], args[1], args[2]);
    }
    return wall;
}

void Wall::update() {

}

void Wall::collision(Entity &entity, int16_t depth) {}

void Wall::draw() {
  Panel::draw(getPosition(), COLOR_WALL_1);
  Panel::draw(getPosition()+getSize()-1, COLOR_WALL_1);
  Panel::drawLine(getPosition(), getSize(), COLOR_WALL_0);
}
