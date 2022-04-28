#include "ExplosionAnimation.h"

#include "VictoryAnimation.h"
#include "../Floor.h"
#include "../Room.h"

#include "../../peripheral/sound/Sound.h"
#include "../../peripheral/led/Panel.h"

#include "../Options.h"
#include "../../Dungeon.h"

#include "../entity/hero/Hero.h"

#define EXPLOSION_SPEED 127

uint8_t ExplosionAnimation::show_idx = 0;
uint8_t ExplosionAnimation::color = 0;
uint8_t ExplosionAnimation::state = 0;
int8_t ExplosionAnimation::substep = 0;
int16_t ExplosionAnimation::pos = 0;
int16_t ExplosionAnimation::size = 0;
int16_t ExplosionAnimation::maxsize = 0;

void ExplosionAnimation::explode(uint8_t color, bool show_idx) {
    if (ExplosionAnimation::isActive()) {        
        return;
    }

    if (color == COLOR_MONSTER_0) {
        Sound::play(Sound::FAILURE);
    } else {
        Sound::play(Sound::SUCCESS);
    }
    ExplosionAnimation::color = color;
    state = 1;
    ExplosionAnimation::show_idx = show_idx;
    substep = 0;
    pos = Hero::position;
    size = 0;
}

void ExplosionAnimation::implode(uint8_t color) {
    if (ExplosionAnimation::isActive() || size <= 0) {        
        return;
    }

    ExplosionAnimation::color = color;
    state = 2;
    maxsize = size;
}

void ExplosionAnimation::door(uint8_t color, int16_t pos) {
    if (ExplosionAnimation::isActive()) {        
        return;
    }

    ExplosionAnimation::color = color;
    ExplosionAnimation::pos = pos;
    state = 3;
    size = 1;
    substep = 0;

    maxsize = size;
}

void ExplosionAnimation::update() {
    if (!ExplosionAnimation::isActive()) {        
        return;
    }

    substep -= EXPLOSION_SPEED;
    while (substep < 0) {
        substep += Options::sub_steps;
        if (state == 1) {
            size += 1;
        } else if (state == 2) {
            size -= 1;
        } else {
            if (Hero::position < pos) {
                pos -= 1;
            } else if (Hero::position > pos) {
                pos += 1;
            }
        }
    }

    if (state == 1) {
        if (pos - size < 0 && pos + size >= Options::level_length) {
            state = 0;
        }
    } else if (state == 2 && size <= 0) {
        state = 0;
    } else if (state == 3 && pos == Hero::position) {
        state = 0;
    }
}

void ExplosionAnimation::drawFloorIndex(int16_t min, int16_t max) {
    int16_t idx = pos - 5;
    if (idx > (int16_t) Options::level_length - 15) {
        idx = Options::level_length - 15;
    } else if (idx < 5) {
        idx = 5;
    }

    if (idx >= max || idx+10 <= min) {
        return;
    }

    if (min < idx) {
        Panel::draw(idx, COLOR_WALL_0);
    }

    idx += 1;
    for (uint8_t i = 0b1; i != 0; i <<= 1, idx += 1) {
        if (idx <= min) {
            continue;
        } else if (idx >= max) {
            return;
        }

        bool on = Floor::idx & i;
        if (on) {
            Panel::draw(idx, color);
        } else {
            Panel::draw(idx, 0);
        }
    }
    Panel::draw(idx, COLOR_WALL_0);
}

void ExplosionAnimation::draw() {
    if (!ExplosionAnimation::isActive()) {
        return;
    }

    int16_t origin = pos;
    if (state == 2) {
        origin = Hero::position + size * (pos - Hero::position) / maxsize;
    }

    Panel::drawLine(origin-size, 2*size, COLOR_OFF, true);
    if (show_idx) {
        ExplosionAnimation::drawFloorIndex(origin-(size>>2), origin+(size>>2));
    }
    
    int16_t offset = size;
    for (uint8_t i = 0; i < 3; i++) {
        Panel::draw(origin+offset, color);
        Panel::draw(origin-offset, color);
        offset >>= 1;
    }
}

bool ExplosionAnimation::isActive() {
    return state;
}

bool ExplosionAnimation::isExplosion() {
    return state == 1;
}

bool ExplosionAnimation::isImplosionOrDoor() {
    return state >= 2;
}
