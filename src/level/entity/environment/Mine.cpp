#include "Mine.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "../hero/Hero.h"
#include "../hero/Sword.h"
#include "../hero/Shot.h"
#include "../monster/Monster.h"
#include "../monster/Worm.h"
#include "../environment/Item.h"

Mine *Mine::spawn(uint8_t *args) {
  return Mine::spawn(args[0], Options::scalePosition(args[1]));
}

Mine *Mine::spawn(uint8_t mask, int16_t pos) {
    Mine *mine = (Mine*) Pool::allocate(MINE_ID);
    if (mine) {
        mine->Entity::spawn(mask, pos);
        mine->blast = 0;
    }
    return mine;
}

void Mine::update() {
  if (blast) {
    blast += 2;
    jump(getPosition()-1);
    if (blast > Options::size_large * 3 / 2 + 1) {
      this->destroy();
    }
  }
}

void Mine::collision(Entity &entity, int8_t depth) {

}

void Mine::collision(Entity &entity) {
  entity.destroy();
  if (!blast) {
    blast = 1;
  }
}

void Mine::collision(Shot &entity, int8_t depth) {
  collision(entity);
}
void Mine::collision(Sword &entity, int8_t depth) {
  collision(entity);
}
void Mine::collision(Hero &entity, int8_t depth) {
  collision(entity);
}
void Mine::collision(Monster &entity, int8_t depth) {
  collision(entity);
}
void Mine::collision(Worm &entity, int8_t depth) {
  // a worm doesn't trigger a mine
  if (!blast) return;
  entity.destroy();
}

void Mine::draw() {
  Panel::drawPeriodic(getPosition(), COLOR_MINE_BEGIN, COLOR_MINE_END);
  if (blast) {
    Panel::drawPeriodic(getPosition()+blast, COLOR_MINE_BEGIN, COLOR_MINE_END);
  }
}

int16_t Mine::getSize() {
  if (blast) {
    return blast;
  }
  return 1;
}
