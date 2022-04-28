#include "Boss.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "../hero/Sword.h"
#include "../hero/Shot.h"
#include "../environment/Item.h"
#include "Monster.h"

Boss *Boss::spawn(uint8_t *args) {
  return Boss::spawn(args[0], Options::scalePosition(args[1]), args[2], args[3]);
}

Boss *Boss::spawn(uint8_t mask, int16_t pos0, uint8_t pos1, uint8_t pos2) {
    Boss *boss = (Boss*) Pool::allocate(BOSS_ID);
    if (boss) {
        boss->Entity::spawn(mask, pos0);
        boss->next_pos = pos1;
        boss->last_pos = pos2;
        boss->length = Options::size_large;
        boss->timer = Timer(0, 80);
    }
    return boss;
}

void Boss::update() {
  Hero::resetAttackType();
  if (timer.dec()) {
    // buffer the data, because changing the
    // pool data, may change this bosses context
    uint8_t mask = getMask();
    uint16_t pos = getPosition()+(length >> 1);

    Monster::spawn(mask, pos, 5);
    Monster::spawn(mask, pos, -5);
  }
}

void Boss::collision(int16_t depth) {
  if (next_pos == 255) {
    length -= abs(depth);
    if (length <= 0) {
      this->destroy();
      Sound::play(Sound::ATTACK);
      Item::spawn(0, getPosition(), ItemType::EXTRA_LIFE);
    } else if (depth < 0) {
      jump(getPosition()-depth);
    }
  } else {
    jump(Options::scalePosition(next_pos));
    next_pos = last_pos;
    last_pos = 255;
    timer.addInterval(-20);
  }
  timer.resetLow();
}

void Boss::collision(Entity &entity, int16_t depth) {}
void Boss::collision(Sword &sword, int16_t depth) {
  collision(depth);
}
void Boss::collision(Shot &shot, int16_t depth) {
  if (depth < 0) depth = -3;
  else           depth = 3;
  collision(depth);
}

void Boss::draw() {
  Panel::drawLine(getPosition(), getSize(), COLOR_MONSTER_1);
}

int16_t Boss::getSize() {
  return length;
}
