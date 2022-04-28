#include "Door.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "../hero/Hero.h"

Door *Door::spawn(uint8_t *args) {
    return Door::spawn(args[0], Options::scalePosition(args[1]), args[2], args[3]);
}

Door *Door::spawn(uint8_t mask, int16_t pos, uint8_t linked_level, uint8_t dst) {
    Door *door = (Door*) Pool::allocate(DOOR_ID);
    if (door) {
        door->Entity::spawn(mask, pos);
        door->linked_level = linked_level;
        door->dst = dst;
    }
    return door;
}

void Door::update() {
}

void Door::collision(Entity &entity, int8_t depth) {}

void Door::draw() {
    Panel::drawLine(getPosition(), getSize(), COLOR_DOOR);
}

int16_t Door::getSize() {
  return Options::size_medium;
}

uint8_t Door::getDst() {
  return dst;
}

uint8_t Door::getLinkedLevel() {
  return linked_level;
}

void Door::enter() {
  Sound::init();
  Sound::play(Sound::TELEPORT);
  
  uint8_t dst = getDst() + 3; // + 3, because a door is 6 in size
  // set new spawn position
  Hero::setCheckpointPos(dst);
  Floor::loadDoor(getLinkedLevel());
}
