#include "Water.h"

#include "../../../peripheral/controller/Controller.h"
#include "../../../peripheral/led/Panel.h"
#include "../../../peripheral/sound/Sound.h"
#include "../../Floor.h"
#include "../../Room.h"
#include "../../Pool.h"
#include "../../Options.h"

Water *Water::spawn(uint8_t *args) {
    Water *water = (Water*) Pool::allocate(WATER_ID);
    if (water) {
        water->LongEntity::spawn(args[0], args[1], args[2]);
        water->current_step = 0;
        water->current_offset = 0;
        water->current_speed = args[3];
    }
    return water;
}

void Water::update() {
    current_step += current_speed;
    while (current_step >= Options::sub_steps) {
        current_step -= Options::sub_steps;
        current_offset++;
        if (current_offset >= 4) {
            current_offset = 0;
        }
    }
    while (current_step < 0) {
        current_step += Options::sub_steps;
        current_offset--;
        if (current_offset < 0) {
            current_offset = 3;
        }
    }
}

void Water::collision(Entity &entity, int16_t depth) {}

void Water::draw() {
    uint8_t palette[] = {COLOR_WATER_0, COLOR_WATER_1, COLOR_WATER_2, COLOR_WATER_3};
    if (current_speed > 0) {
        palette[0] = COLOR_WATER_3;
        palette[1] = COLOR_WATER_2;
        palette[2] = COLOR_WATER_1;
        palette[3] = COLOR_WATER_0;
    }

    Panel::drawLinePattern(getPosition(), getSize(), current_offset, &palette[0], 4);

    // the hero is kind of hard to see when in water (especially when blue)
    // => draw the water close to the hero in a dim color
    int16_t p0 = getPosition();
    int16_t p1 = Hero::position - (Options::size_medium+2);
    int16_t p2 = Hero::position + (Options::size_medium+2);
    int16_t p3 = getPosition() + getSize();
    int16_t offset = 0;
    if (p1 > p0) {
      offset += p1 - p0;
      p0 = p1;
    }
    if (p2 < p3) {
      p3 = p2;
    }
    if (p0 < p3) {
        if (palette[0] == COLOR_WATER_0) {
            palette[0] = COLOR_WATER_4;
        } else {
            palette[3] = COLOR_WATER_4;
        }
        palette[1] = COLOR_WATER_4;
        palette[2] = COLOR_WATER_4;
        Panel::drawLinePattern(p0, p3-p0, (current_offset - offset)&0b11, &palette[0], 4);
    }
}

int8_t Water::getCurrent() {
    return current_speed;
}
