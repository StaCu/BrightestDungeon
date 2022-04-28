#include "Dungeon.h"

#include "Settings.h"

#include "level/Pool.h"
#include "level/entity/hero/Hero.h"
#include "level/Room.h"
#include "level/Options.h"
#include "level/loader/FileLoader.h"
#include "level/Floor.h"
#include "level/animation/ExplosionAnimation.h"
#include "level/animation/VictoryAnimation.h"

#include "peripheral/controller/Controller.h"
#include "peripheral/Pin.h"
#include "peripheral/sound/Sound.h"
#include "peripheral/led/Panel.h"

void Dungeon::setup() {
    wdt_disable();
    Pin::setModeDigitalRead<OPTIONS_PIN>();
    Pin::writeHigh<OPTIONS_PIN>();
    #if ENABLE_LOGGING
    #ifndef SIMULATOR
        Serial.begin(BAUDRATE);
    #endif
    #endif
    LOG_LN(F("init:"));
    while (millis() < 1000) {
        // don't turn the leds on immediatly
    }
    Options::read();
    Controller::init();
    Sound::init();
    Panel::init();
    #if ENABLE_WATCHDOG
        wdt_enable(WATCHDOG_INTERVAL);
    #endif
    LOG_LN(F("done"));
}

void Dungeon::ensureFrameTiming(uint16_t &loop_start_time) {
    #if LOG_TIMING_SLACK
        uint16_t LOOP_END_TIME = millis();
        uint16_t delta = LOOP_END_TIME - loop_start_time;
        LOG("sleep: ");
        LOG_LN(UPDATE_TIME - delta);
        delay(UPDATE_TIME - delta);
    #else
        uint16_t delta;
        while (true) {
            uint16_t LOOP_END_TIME = millis();
            delta = LOOP_END_TIME - loop_start_time;
            if (delta > UPDATE_TIME) {
                break;
            }
        }
    #endif
    do {
        loop_start_time += UPDATE_TIME;
        delta -= UPDATE_TIME;
    } while ((int16_t) delta > UPDATE_TIME);
}

void Dungeon::run() {
    Floor::idx = Options::start_floor;
    Floor::loadImmediately();

    uint16_t loop_start_time = millis();
    uint16_t last_input_time = loop_start_time >> 1;

    while (true) {
        Panel::clear();
        uint16_t time_since_last_input = ((uint16_t) (millis() >> 1)) - last_input_time;
        if (Dungeon::isIdle()) {
            if (time_since_last_input > (STAND_BY_TIMEOUT_RESET >> 1)) {
                Floor::idx = Options::start_floor;
            }
        } else if (time_since_last_input > (STAND_BY_TIMEOUT >> 1) && !Dungeon::isSecret()) {
            Floor::loadIdle();
        } else if (!Pin::read<OPTIONS_PIN>()) {
            if (Dungeon::isOptions()) {
                Floor::loadFailure();
            } else {
                Floor::loadOptions();
            }
        }
        if (Controller::poll()) {
            last_input_time = millis() >> 1;
            if (Dungeon::isIdle()) {
                Floor::loadImmediately();
            }
        }
        Floor::update();
        Floor::draw();
#ifdef SIMULATOR
        Panel::show();
        Sound::update();
        Simulator::update();
        ensureFrameTiming(loop_start_time);
        wdt_reset();
        if (Simulator::poll()) {
            Simulator::destroy();
            break;
        }
#else
        Sound::update();
        Panel::show();
        ensureFrameTiming(loop_start_time);
        wdt_reset();
#endif
    }
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
