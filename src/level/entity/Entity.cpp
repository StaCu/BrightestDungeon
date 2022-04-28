#include "Entity.h"

#include "../Floor.h"
#include "../Pool.h"

#include "EntityUnion.h"

uint8_t Entity::global_mask;

bool Entity::isActive() const {
    return (mask & global_mask) == mask;
}

void Entity::resetMask() {
    Entity::global_mask = 0xF0;
}
  
void Entity::resetMaskPartially(uint8_t mask) {
    Entity::global_mask = (Entity::global_mask & ~mask) | (0xF0 & mask);
}

Entity *Entity::spawn(uint8_t type, uint8_t *args) {
    switch (type) {
    case HERO_ID:       return Hero::spawn(args);
    //case SHOT_ID:       return Shot::spawn(args);
    //case SWORD_ID:      return Sword::spawn(args);

    case BOSS_ID:       return Boss::spawn(args);
    case MONSTER_ID:    return Monster::spawn(args);
    case WORM_ID:       return Worm::spawn(args);
    case SPAWNER_ID:    return Spawner::spawn(args);

    case BUTTON_ID:     return Button::spawn(args);
    case DOOR_ID:       return Door::spawn(args);
    case FOG_ID:        return Fog::spawn(args);
    case FORCEFIELD_ID: return ForceField::spawn(args);
    case ITEM_ID:       return Item::spawn(args);
    case LAVA_ID:       return Lava::spawn(args);
    case MINE_ID:       return Mine::spawn(args);
    case PORTAL_ID:     return Portal::spawn(args);
    case SWITCH_ID:     return Switch::spawn(args);
    case WALL_ID:       return Wall::spawn(args);
    case WATER_ID:      return Water::spawn(args);
    }
    return nullptr;
}

void Entity::spawn(uint8_t mask, int16_t pos) {
    this->jump(pos);
    this->mask = mask;
}

void Entity::destroy() {
  type = ENTITY_TYPE_COUNT;
  //Pool::free(this);
}

void Entity::update() {

}

void Entity::draw() {

}

uint8_t Entity::getType() const {
  return type;
}

uint8_t Entity::getMask() {
  return mask;
}

void Entity::jump(int16_t pos) {
  this->pos = pos;
}

int16_t Entity::getPosition() {
    return pos;
}

int16_t Entity::getOldPosition() {
    return pos;
}

int16_t Entity::getSize() {
    return 1;
}

uint8_t Entity::getArgCount(uint8_t type) {
  switch (type) {
  case HERO_ID:       return 2;
  //case SHOT_ID:       return 2;
  //case SWORD_ID:      return 2;

  case BOSS_ID:       return 4;
  case MONSTER_ID:    return 3;
  case WORM_ID:       return 3;
  case SPAWNER_ID:    return 5;

  case BUTTON_ID:     return 5;
  case DOOR_ID:       return 4;
  case FOG_ID:        return 4;
  case FORCEFIELD_ID: return 3;
  case ITEM_ID:       return 3;
  case LAVA_ID:       return 5;
  case MINE_ID:       return 2;
  case PORTAL_ID:     return 3;
  case SWITCH_ID:     return 3;
  case WALL_ID:       return 3;
  case WATER_ID:      return 4;
  default: return 0;
  }
}
