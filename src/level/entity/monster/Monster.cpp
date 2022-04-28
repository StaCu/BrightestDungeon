#include "Monster.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"
#include "../../../Dungeon.h"

#include "../hero/Sword.h"
#include "../hero/Shot.h"
#include "../environment/ForceField.h"
#include "../environment/Lava.h"
#include "../environment/Wall.h"

Monster *Monster::spawn(uint8_t *args) {
    return Monster::spawn(args[0], Options::scalePosition(args[1]), args[2]);
}

Monster *Monster::spawn(uint8_t mask, int16_t pos, int8_t speed) {
    Monster *monster = (Monster*) Pool::allocate(MONSTER_ID);
    if (monster) {
        monster->MovingEntity::spawn(mask, pos);
        monster->speed = speed;
    }
    return monster;
}

void Monster::update() {
  MovingEntity::update();
  int8_t side = move(speed);
  if (side != 0) {
    speed = 0 - speed;
    if ((Dungeon::isVictory() || Dungeon::isIdle()) && side > 0) {
      destroy();
    }
  }
}

void Monster::collision(Entity &entity, int16_t depth) {}

void Monster::collision(Sword &sword, int16_t depth) {
  Sound::play(Sound::ATTACK);
  destroy();
}
void Monster::collision(Shot &shot, int16_t depth) {
  Sound::play(Sound::ATTACK);
  destroy();
}
void Monster::collision(Block &block, int16_t depth) {
  jump(getPosition()-depth);
  speed = 0 - speed;
}
void Monster::collision(ForceField &forcefield, int16_t depth) {
  destroy();
}
void Monster::collision(Lava &lava, int16_t depth) {
  if (lava.isOn()) {
    destroy();
  }
}
void Monster::collision(Wall &wall, int16_t depth) {
  jump(getPosition()-depth);
  speed = 0 - speed;
}

void Monster::draw() {
  Panel::draw(getPosition(), COLOR_MONSTER_0);
}

int16_t Monster::getSize() {
  return 1;
}
