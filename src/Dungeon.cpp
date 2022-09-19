#include "Dungeon.h"

#include <Platform.h>

#include "level/Pool.h"
#include "level/entity/hero/Hero.h"
#include "level/Room.h"
#include "level/Options.h"
#include "level/loader/FileLoader.h"
#include "level/Floor.h"
#include "level/animation/ExplosionAnimation.h"
#include "level/animation/VictoryAnimation.h"

#include "peripheral/controller/Controller.h"
#include "peripheral/sound/Sound.h"
#include "peripheral/led/Panel.h"

void Dungeon::setup() {
    platform_init();
    platform_watchdog_disable();
    #if ENABLE_LOGGING
        platform_log_init();
    #endif
    LOG_LN(F("init:"));
    while (platform_millis() < STARTUP_SAFETY_TIME) {
        // don't turn the leds on immediatly
    }
    Options::init();
    Controller::init();
    Sound::init();
    Panel::init(); 
    #if ENABLE_WATCHDOG
        platform_watchdog_enable();
    #endif
    LOG_LN(F("done"));
}

void Dungeon::ensureFrameTiming(uint16_t &loop_start_time) {
    uint16_t LOOP_END_TIME = platform_millis();
    uint16_t delta = LOOP_END_TIME - loop_start_time;
    #if LOG_TIMING_SLACK
        LOG("slack: ");
        LOG_LN(UPDATE_TIME - delta);
    #endif
    while (delta <= UPDATE_TIME) {
        LOOP_END_TIME = platform_millis();
        delta = LOOP_END_TIME - loop_start_time;
    }
    
    do {
        loop_start_time += UPDATE_TIME;
        delta -= UPDATE_TIME;
    } while ((int16_t) delta > UPDATE_TIME);
}

void Dungeon::run() {
    Floor::idx = Options::start_floor;
    Floor::loadImmediately();

    uint16_t loop_start_time = platform_millis();
    uint16_t last_input_time = loop_start_time >> 1;

    while (true) {
        Panel::clear();
        uint16_t time_since_last_input = ((uint16_t) (platform_millis() >> 1)) - last_input_time;
        if (Dungeon::isIdle()) {
            if (time_since_last_input > (STAND_BY_TIMEOUT_RESET >> 1)) {
                Floor::idx = Options::start_floor;
            }
        } else if (time_since_last_input > (STAND_BY_TIMEOUT >> 1) && !Dungeon::isSecret()) {
            Floor::loadIdle();
        } else if (Controller::pollOptions()) {
            if (Dungeon::isOptions()) {
                Floor::loadFailure();
            } else {
                Floor::loadOptions();
            }
        }
        if (Controller::poll()) {
            last_input_time = platform_millis() >> 1;
            if (Dungeon::isIdle()) {
                Floor::loadImmediately();
            }
        }
        Floor::update();
        Floor::draw();
        Sound::update();
        Panel::show();
        ensureFrameTiming(loop_start_time);
        platform_update();
        platform_poll();
        if (controller_poll_turn_off()) {
            break;
        }
        platform_watchdog_reset();
    }
    platform_deinit();
}

bool Dungeon::isIdle() {
    return Options::isIdle();
}

bool Dungeon::isSecret() {
    return Sound::getType() == Sound::SECRET;
}

bool Dungeon::isOptions() {
    return Options::isOptions();
}

bool Dungeon::isVictory() {
    return VictoryAnimation::isActive();
}

bool Dungeon::isSuccess() {
    return Hero::atGoal();
}

bool Dungeon::isFailure() {
    return !Hero::isAlive();
}
