#include "Floor.h"

#include <Platform.h>

#include "entity/environment/Door.h"

#include "../Dungeon.h"
#include "Pool.h"
#include "Room.h"
#include "Options.h"

#include "entity/hero/Hero.h"
#include "entity/hero/Shot.h"
#include "entity/environment/Item.h"

#include "../peripheral/sound/Sound.h"
#include "../peripheral/led/Panel.h"
#include "../util/Timer.h"

#include "animation/ExplosionAnimation.h"
#include "animation/VictoryAnimation.h"

uint8_t Floor::idx = 0;
uint8_t Floor::room_idx = 0;

Floor::State Floor::state;

void Floor::update() {
    Timer::updateTimer();

    if (!ExplosionAnimation::isImplosionOrDoor() && state != State::START_TIMER) {
        Room::update();
        Options::update();
    }

    ExplosionAnimation::update();
    VictoryAnimation::update();

    if (!ExplosionAnimation::isActive() && !VictoryAnimation::isActive() && state != State::PLAY) {
        Floor::load();
    }
}

void Floor::draw() {
    Room::draw();

    VictoryAnimation::draw();
    ExplosionAnimation::draw();
    
    Panel::drawLives(Hero::lives);

    if (Hero::hasGoal() && !Pool::contains(BOSS_ID)) {
        Panel::draw(Options::level_length-1, Hero::getColorIdx());
    }
}

void Floor::resetAll() {
    Floor::room_idx = 0;
    Entity::resetMask();
    if (Options::isOptions()) {
        Options::write();
    }
    if (Floor::idx == Options::start_floor) {
        // in this case, it is okay to break with the color-continuity
        // across floors to ensure the first level always looks the same
        Options::swap_color = false;
    }
}

void Floor::load() {
    switch (state) {
    case LOAD_CHECKPOINT: {
        LOG_LN(F("LOAD_CHECKPOINT"));
        Floor::resetAll();
        Hero::resetAll();
        Room::load(0);
        ExplosionAnimation::implode(Hero::getColorIdx());
        state = State::START_TIMER;
    } break;
    case LOAD_SUCCESS: {
        LOG_LN(F("LOAD_SUCCESS"));
        Hero::resetSuccess();
        Floor::room_idx = 0;
        Entity::resetMask();
        state = State::START_TIMER;
        if ((Floor::idx == 0 && !Options::isOptions()) || !Room::load(0)) {
            Pool::reset();
            loadVictory();
        }
        if (!VictoryAnimation::isActive()) {
            ExplosionAnimation::implode(Hero::getColorIdx());
        }
    } break;
    case LOAD_FAILURE: {
        LOG_LN(F("LOAD_FAILURE"));
        Hero::resetFailure();
        Room::load(0);
        ExplosionAnimation::implode(Hero::getColorIdx());
        state = State::START_TIMER;
    } break;
    case LOAD_IDLE: {
        LOG_LN(F("LOAD_IDLE"));
        Floor::resetAll();
        Hero::resetAll();
        Room::load(2);
        state = State::START_TIMER;
    } break;
    case LOAD_OPTIONS: {
        LOG_LN(F("LOAD_OPTIONS"));
        Floor::resetAll();
        Hero::resetAll();
        Room::load(1);
        ExplosionAnimation::implode(Hero::getColorIdx());
        state = State::START_TIMER;
    } break;
    case LOAD_DOOR: {
        LOG_LN(F("LOAD_DOOR"));
        uint16_t pos = Hero::position;
        if (!Room::load(0)) {
            loadVictory();
        } else {
            ExplosionAnimation::door(Hero::getColorIdx(), pos);
            state = START_TIMER;
        }
    } break;
    case START_TIMER: {
        LOG_LN(F("START_TIMER"));
        Timer::resetTimer();
        Options::stopCombo();
        state = PLAY;
    } break;
    default: break;
    }
}

void Floor::loadOptions() {
    if (state != PLAY) return;
    LOG_LN(F("load options"));
    ExplosionAnimation::explode(COLOR_MONSTER_0, false);
    Floor::state = LOAD_OPTIONS;
}

void Floor::loadIdle() {
    if (state != PLAY) return;
    LOG_LN(F("load idle"));
    ExplosionAnimation::explode(COLOR_MONSTER_0, false);
    if (!Options::checkpoints) {
        Floor::idx = Options::start_floor;
    }
    Floor::state = LOAD_IDLE;
}

void Floor::loadVictory() {
    LOG_LN(F("load victory"));
    Floor::idx = Options::start_floor;
    VictoryAnimation::start();
    state = LOAD_CHECKPOINT;
}

void Floor::loadImmediately() {
    if (state != PLAY) return;
    LOG_LN(F("load checkpoint immediately"));
    Floor::state = LOAD_CHECKPOINT;
}

void Floor::loadDoor(uint8_t room) {
    if (state != PLAY) return;
    LOG_LN(F("load door"));
    room_idx = room;
    Floor::state = LOAD_DOOR;
}

void Floor::loadSuccess() {
    if (state != PLAY) return;
    LOG_LN(F("load success"));
    ExplosionAnimation::explode(Hero::getColorIdx());
    if (!Options::isOptions()) {
        Floor::idx += 1;
    }
    state = LOAD_SUCCESS;
}

void Floor::loadFailure() {
    if (state != PLAY) return;
    LOG_LN(F("load failure"));
    if (!Options::isOptions()) {
        Hero::looseLive();
    }
    ExplosionAnimation::explode(COLOR_MONSTER_0);
    if (Hero::lives == 0) {
        if (!Options::checkpoints) {
            Floor::idx = Options::start_floor;
        }
        state = LOAD_CHECKPOINT;
    } else {
        state = LOAD_FAILURE;
    }
}
