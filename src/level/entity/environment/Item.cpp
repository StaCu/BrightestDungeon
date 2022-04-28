#include "Item.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "../hero/Hero.h"

const PROGMEM uint8_t Item::led_item_life[] {
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM        , COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM        , COLOR_ITEM_COLOR_1, COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM        , COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_ITEM_COLOR_1, COLOR_ITEM        , COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM        , COLOR_ITEM_COLOR_0, COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM        , COLOR_ITEM_COLOR_0, COLOR_ITEM_COLOR_1, 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM        , COLOR_ITEM_COLOR_1, COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM_COLOR_1, COLOR_ITEM_COLOR_1, COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM_COLOR_0, COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM_COLOR_0, COLOR_ITEM_COLOR_1, COLOR_OFF         , 
  COLOR_OFF         , COLOR_OFF         , COLOR_ITEM_COLOR_1, COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_ITEM_COLOR_1, COLOR_ITEM_COLOR_0, COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_ITEM_COLOR_0, COLOR_ITEM        , COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_ITEM_COLOR_0, COLOR_ITEM        , COLOR_OFF         , COLOR_OFF         , 
  COLOR_OFF         , COLOR_ITEM_COLOR_1, COLOR_ITEM        , COLOR_OFF         , COLOR_OFF         , 
  COLOR_ITEM_COLOR_1, COLOR_ITEM_COLOR_0, COLOR_ITEM        , COLOR_OFF         , COLOR_OFF         , 
};

Item *Item::spawn(uint8_t *args) {
    return Item::spawn(args[0], Options::scalePosition(args[1]), (ItemType) args[2]);
}

Item *Item::spawn(uint8_t mask, int16_t pos, ItemType type) {
    Item *item = (Item*) Pool::allocate(ITEM_ID);
    if (item) {
        item->Entity::spawn(mask, pos);
        item->type = type;
    }
    return item;
}

void Item::update() {
}

void Item::collision(Entity &entity, int16_t depth) {}
void Item::collision(Hero &hero, int16_t depth) {
  if (!(type == SET_COLOR_0 || type == SET_COLOR_1)) {
    this->destroy();
  }
}

void Item::draw() {
  if (type == SET_COLOR_0 || type == SET_COLOR_1) {
    int8_t timer = Panel::getTimer();
    uint8_t absolute = abs(timer);
    if (absolute < 16) {
      Panel::draw(getPosition(), COLOR_WALL_1);
    } else if (absolute < 32) {
      Panel::draw(getPosition()-1, COLOR_WALL_1);
      Panel::draw(getPosition()+1, COLOR_WALL_1);
    } else {
      Panel::draw(getPosition()-2, COLOR_WALL_1);
      if (type == SET_COLOR_0 != Options::swap_color) {
        Panel::drawLine(getPosition()-1, 3, COLOR_ITEM_COLOR_0);
      } else {
        Panel::drawLine(getPosition()-1, 3, COLOR_ITEM_COLOR_1);
      }
      Panel::draw(getPosition()+2, COLOR_WALL_1);
    }
    return;
  }
  if (type == SWAP_ATTACK_TYPE) {
    uint8_t timer = Panel::getTimer();
    bool sword = timer >= 128;
    if (sword) timer -= 128;
    timer /= 32;
    if (timer <= 1) {
      if (sword) {
        Panel::drawLine(getPosition()-2, 5, COLOR_ITEM_ATTACK);
      } else {
        Panel::draw(getPosition()-timer-1, COLOR_ITEM_ATTACK);
        Panel::draw(getPosition()+timer+1, COLOR_ITEM_ATTACK);
      }
    }
    Panel::draw(getPosition(), COLOR_ITEM);
    return;
  }
  if (type == INVERT_CONTROL) {
    int8_t timer = Panel::getTimer();
    uint8_t absolute = abs(timer);
    // divide into 5 steps
    absolute /= (128 / 5);

    Panel::draw(getPosition()-2+absolute, (timer < 0) ? COLOR_ITEM_INVERT_CONTROL_0 : COLOR_ITEM_INVERT_CONTROL_1);
    Panel::draw(getPosition(), COLOR_ITEM);
    return;
  }
  if (type == EXTRA_LIFE) {
    uint8_t idx = (Panel::getTimer() >> 4) * 5;
    uint8_t colors[5];
    for (uint8_t i = 0; i < 5; i++) {
      colors[i] = pgm_read_byte_near(&led_item_life[idx+i]);
    }
    Panel::drawLinePattern(getPosition()-2, 5, 0, colors, 5, false);
  }
}

int16_t Item::getSize() {
  return 1;
}

ItemType Item::getType() {
  return type;
}
