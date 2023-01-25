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

uint8_t ExplosionAnimation::type = 0;
uint8_t ExplosionAnimation::color = 0;
uint8_t ExplosionAnimation::state = State::OFF;
int8_t ExplosionAnimation::substep = 0;
int16_t ExplosionAnimation::pos = 0;
int16_t ExplosionAnimation::size = 0;
int16_t ExplosionAnimation::maxsize = 0;

void ExplosionAnimation::explode(uint8_t color, uint8_t type) {
    if (ExplosionAnimation::isActive()) {        
        return;
    }

    ExplosionAnimation::color = color;
    if (type & Type::DELAYED) {
        state = State::LOOSE;
    } else {
        state = State::EXPLODE-1;
    }
    ExplosionAnimation::type = type;
    substep = 0;
    pos = Hero::position;
    size = 0;
}

void ExplosionAnimation::implode(uint8_t color) {
    if (ExplosionAnimation::isActive() || size <= 0) {        
        return;
    }

    ExplosionAnimation::color = color;
    state = State::IMPLODE;
    maxsize = size;
}

void ExplosionAnimation::door(uint8_t color, int16_t pos) {
    if (ExplosionAnimation::isActive()) {        
        return;
    }

    ExplosionAnimation::color = color;
    ExplosionAnimation::pos = pos;
    state = State::DOOR;
    size = 1;
    substep = 0;

    maxsize = size;
}

void ExplosionAnimation::update() {
    if (!ExplosionAnimation::isActive()) {        
        return;
    }

    if (state == State::EXPLODE-1) {
        if (color == COLOR_MONSTER_0) {
            Sound::play(Sound::FAILURE);
        } else {
            Sound::play(Sound::SUCCESS);
        }
    }
    if (state < State::EXPLODE) {
        state += 1;
        return;
    }

    substep -= EXPLOSION_SPEED;
    while (substep < 0) {
        substep += Options::sub_steps;
        if (state == State::EXPLODE) {
            size += 1;
        } else if (state == State::IMPLODE) {
            size -= 1;
        } else {
            if (Hero::position < pos) {
                pos -= 1;
            } else if (Hero::position > pos) {
                pos += 1;
            }
        }
    }

    if (state == State::EXPLODE) {
        if (type & Type::LARGE) {
            if (size > Options::level_length * 3) {
                state = State::OFF;
            }
        } else if (pos - size < 0 && pos + size >= Options::level_length) {
            state = State::OFF;
        }
    } else if (state == State::IMPLODE && size <= 0) {
        state = State::OFF;
    } else if (state == State::DOOR && pos == Hero::position) {
        state = State::OFF;
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
    if (state == State::IMPLODE) {
        origin = Hero::position + size * (pos - Hero::position) / maxsize;
    }

    Panel::drawLine(origin-size, 2*size, COLOR_OFF, true);
    if (type & Type::SHOW_LEVEL) {
        uint8_t shift = type & Type::LARGE ? 4 : 2;
        ExplosionAnimation::drawFloorIndex(origin-(size>>shift), origin+(size>>shift));
    }
    
    int16_t offset = size;
    uint8_t count = type & Type::LARGE ? 5 : 3;
    for (uint8_t i = 0; i < count; i++) {
        Panel::draw(origin+offset, color);
        Panel::draw(origin-offset, color);
        offset >>= 1;
    }
}

bool ExplosionAnimation::isActive() {
    return state != State::OFF;
}

bool ExplosionAnimation::isExplosion() {
    return state >= State::LOOSE && state <= State::EXPLODE;
}

bool ExplosionAnimation::isImplosionOrDoor() {
    return state >= State::IMPLODE;
}
