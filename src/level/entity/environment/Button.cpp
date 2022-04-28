#include "Button.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

#include "../hero/Hero.h"
#include "Door.h"

Button *Button::spawn(uint8_t *args) {
  return Button::spawn(args[0], Options::scalePosition(args[1]), args[2], args[3], args[4]);
}

Button *Button::spawn(uint8_t mask, int16_t pos, uint8_t operation, uint8_t first, uint8_t interval) {
    Button *button = (Button*) Pool::allocate(BUTTON_ID);
    if (button) {
        button->Entity::spawn(mask, pos);
        button->operation = operation;
        button->timer = Timer(first, interval);
        button->used = false;
    }
    return button;
}

void Button::update() {
    if (used) {
        if (timer.dec()) {
            used = false;
            Entity::global_mask ^= operation;
        }
    }
}

void Button::collision(Entity &entity, int8_t depth) {
    if (!used) {
        Entity::global_mask ^= operation;
        used = true;
    }
    timer.resetHigh();
}

void Button::draw() {
    uint8_t color = COLOR_SWITCH_ON;
    if (used) {
        color = COLOR_SWITCH_OFF;
    }
    Panel::drawLine(getPosition(), getSize(), color);
}

int16_t Button::getSize() {
  return Options::size_medium;
}
