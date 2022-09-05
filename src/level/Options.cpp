#include "Options.h"

#include <Platform.h>

#include "Floor.h"
#include "Pool.h"

#include "../peripheral/led/Panel.h"
#include "../peripheral/controller/Controller.h"

#include "entity/hero/Hero.h"
#include "entity/environment/Door.h"

// # ================================================================ #
// # ::  OPTIONS                                                   :: #
// # ================================================================ #

uint8_t Options::dungeon;
uint8_t Options::start_floor;
bool Options::checkpoints;

int16_t Options::led_count;
uint8_t Options::brightness;
uint8_t Options::volume;

uint8_t Options::controller_direction;

bool Options::swap_color;
int16_t Options::level_length;
int8_t Options::sub_steps;
int8_t Options::size_large;
int8_t Options::size_medium;
int8_t Options::size_small;

void Options::init() {
    #if ENABLE_RUNTIME_SETTINGS
    settings_init();
    #endif
    Options::read();
}

struct RuntimeSettings {
    uint8_t initiated;
    uint8_t dungeon;
    uint8_t start_floor;
    bool checkpoints;

    int16_t led_count;
    uint8_t brightness;
    uint8_t volume;

    uint8_t controller_direction;
};

void Options::read() {
    uint8_t initiated = DEFAULT_TESTVALUE+1;
    #if ENABLE_RUNTIME_SETTINGS
    RuntimeSettings settings;
    settings_read(&settings, sizeof(RuntimeSettings));
    settings_read_platform_specific(sizeof(RuntimeSettings));
    initiated            = settings.initiated;
    dungeon              = settings.dungeon;
    start_floor          = settings.start_floor;
    checkpoints          = settings.checkpoints;
    led_count            = settings.led_count;
    brightness           = settings.brightness;
    volume               = settings.volume;
    controller_direction = settings.controller_direction;

    LOG("eeprom: ");
    LOG((int)initiated);
    LOG("|");
    LOG((int)Options::dungeon);
    LOG(" ");
    LOG((int)Options::start_floor);
    LOG(" ");
    LOG((int)Options::checkpoints);
    LOG("|");
    LOG((int)Options::led_count);
    LOG(" ");
    LOG((int)Options::brightness);
    LOG(" ");
    LOG((int)Options::volume);
    LOG("|");
    LOG_LN((int)Options::controller_direction);
    #endif

    if (initiated != DEFAULT_TESTVALUE) {
        dungeon      = DEFAULT_DUNGEON;
        start_floor  = DEFAULT_FIRST_FLOOR;
        checkpoints  = DEFAULT_CHECKPOINTS;

        led_count    = DEFAULT_LED_COUNT;
        brightness   = DEFAULT_BRIGHTNESS;
        volume       = DEFAULT_VOLUME;
        
        controller_direction = DEFAULT_DIRECTION;
        settings_default();
    }

    // sanitize settings
    if (dungeon    > 15)                                               dungeon    = DEFAULT_DUNGEON;
    if (led_count  < LED_MIN_COUNT || LED_MAX_COUNT      < led_count)  led_count  = DEFAULT_LED_COUNT;
    if (brightness < 0             || LED_MAX_BRIGHTNESS < brightness) brightness = DEFAULT_BRIGHTNESS;
    if (volume     < 0             || SOUND_MAX_VOLUME   < volume)     volume     = DEFAULT_VOLUME;
    if (Options::controller_direction) {
        Options::controller_direction = true;
    }
    settings_check();

    Options::updateLevelLength();
    Palette::updateBrightness();
    tmp_floor = start_floor;
}

void Options::write() {
    dungeon     = tmp_dungeon;
    Floor::idx  = tmp_floor;
    start_floor = tmp_floor;
    #if ENABLE_RUNTIME_SETTINGS
    RuntimeSettings settings;
    settings.initiated            = DEFAULT_TESTVALUE;
    settings.dungeon              = dungeon;
    settings.start_floor          = start_floor;
    settings.checkpoints          = checkpoints;
    settings.led_count            = led_count;
    settings.brightness           = brightness;
    settings.volume               = volume;
    settings.controller_direction = controller_direction;
    settings_write(&settings, sizeof(RuntimeSettings));
    settings_write_platform_specific(sizeof(RuntimeSettings));
    #endif
}

int16_t Options::scalePosition(uint8_t pos) {
  return (((uint32_t) pos) * Options::level_length) / 250;
}

void Options::updateLevelLength() {
  Options::level_length = Options::led_count - (1 + MAX_LIVES + LED_STATUS_COUNT);
  Options::sub_steps    = ((32 * 250) / Options::level_length);
  Options::size_large   = scalePosition(16);
  Options::size_medium  = scalePosition(6);
  Options::size_small   = scalePosition(4);
}

// # ================================================================ #
// # ::  SELECTION                                                 :: #
// # ================================================================ #

uint8_t Options::use;
Options::State Options::state;

uint8_t Options::tmp_dungeon;
uint8_t Options::tmp_floor;

uint8_t Options::getSelection() {
    if (use < 0b10000000) {
        return 255;
    }

    uint8_t size = Options::size_medium + 1;
    uint8_t slot = Hero::position / size;
    uint8_t dot  = Hero::position % size;
    if (dot == Options::size_medium) {
        return 255;
    }
    return slot;
}

void Options::select() {
    if (use == 0) {
        use = 0b10000001;
    } else if (use < 0b10010000) {
        use = (use << 1) | 0b10000000;
    }
}

void Options::stopCombo() {
    use = 0;
}

// # ================================================================ #
// # ::  STATE                                                     :: #
// # ================================================================ #

void Options::setIdle() {
    Options::state = State::STATE_IDLE;
}

bool Options::isIdle() {
    return Options::state == State::STATE_IDLE;
}

void Options::setOptions() {
    Options::state = State::STATE_MAIN;
    Hero::spawn(0, 0);
    Hero::setGoal(true);
    tmp_dungeon = dungeon;
    tmp_floor   = Floor::idx;
    use = 0;
}

bool Options::isOptions() {
    return Options::state == State::STATE_MAIN;
}

void Options::off() {
    if (isOptions()) {
        Options::dungeon = tmp_dungeon;
        Floor::idx       = tmp_floor;
        start_floor      = tmp_floor;
    }
    Options::state   = State::STATE_OFF;
}

// # ================================================================ #
// # ::  UPDATE                                                    :: #
// # ================================================================ #

void Options::update() {
    if (!isOptions()) {
        return;
    }

    uint8_t selection = getSelection();
    if (selection == OptionsMain::CALLIBRATE) {
        Controller::callibrate();
    }
    if (selection == OptionsMain::CONTROLLER_DIRECTION) {
	    Options::controller_direction = !Options::controller_direction;
    }
    // ---
    if (selection == OptionsMain::DECR_LED_COUNT) {
        Options::led_count -= use & 0b01111111;
        if (Options::led_count < LED_MIN_COUNT) {
            Options::led_count = LED_MIN_COUNT;
        }
        Options::updateLevelLength();
        setHeroPosition(3);
    }
    if (selection == OptionsMain::INCR_LED_COUNT) {
        Options::led_count += use & 0b01111111;
        if (Options::led_count > LED_MAX_COUNT) {
            Options::led_count = LED_MAX_COUNT;
        }
        Options::updateLevelLength();
        setHeroPosition(4);
    }
    if (selection == OptionsMain::DECR_BRIGHTNESS) {
        if (Options::brightness != 0) {
            Options::brightness -= 1;
            Palette::updateBrightness();
        }
    }
    if (selection == OptionsMain::INCR_BRIGHTNESS) {
        if (Options::brightness < LED_MAX_BRIGHTNESS) {
            Options::brightness += 1;
            Palette::updateBrightness();
        }
    }
    if (selection == OptionsMain::DECR_VOLUME) {
        if (Options::volume != 0) {
            Options::volume -= 1;
        }
    }
    if (selection == OptionsMain::INCR_VOLUME) {
        if (Options::volume < SOUND_MAX_VOLUME) {
            Options::volume += 1;
        }
    }
    // ---
    if (selection == OptionsMain::CHECKPOINTS) {
        Options::checkpoints = !Options::checkpoints;
    }
    // ---
    if (selection >= OptionsMain::DUNGEON0 && selection <= OptionsMain::DUNGEON3) {
        Options::tmp_dungeon ^= (0b1 << (selection-OptionsMain::DUNGEON0));
        if (Options::tmp_dungeon == Options::dungeon) {
            Options::tmp_floor = Floor::idx;
        } else {
            Options::tmp_floor = 0;
        }
    }
    // ---
    if (selection >= OptionsMain::FLOOR0 && selection <= OptionsMain::FLOOR7) {
        Options::tmp_floor ^= (0b1 << (selection-OptionsMain::FLOOR0));
    }
    // ---
    if (selection == OptionsMain::READ_OPTIONS) {
        Options::read();
    }
    if (selection == OptionsMain::WRITE_OPTIONS) {
        Options::write();
    }
    // ---
    use &= 0b01111111;
}

void Options::setHeroPosition(uint8_t slot) {
    int16_t pos = slot * (Options::size_medium+1) + Options::size_medium/2;
    Entity *hero = Pool::getFirst(HERO_ID);
    hero->jump(pos);
    Hero::position = pos;
}

// # ================================================================ #
// # ::  DRAW                                                      :: #
// # ================================================================ #

void Options::draw() {
    if (!isOptions()) {
        return;
    }

    uint8_t  size = Options::size_medium;
    uint16_t pos  = 0;

    for (uint8_t i = 0; i <= OptionsMain::SPACER5; i++, pos += size + 1) {
        uint8_t color = COLOR_WALL_1;
        if (i >= OptionsMain::DUNGEON0 && i <= OptionsMain::DUNGEON3 && !(Options::tmp_dungeon & (1<<(i-OptionsMain::DUNGEON0)))) {
            color = COLOR_WALL_0;
        } else
        if (i >= OptionsMain::FLOOR0 && i <= OptionsMain::FLOOR7 && !(Options::tmp_floor & (1<<(i-OptionsMain::FLOOR0)))) {
            color = COLOR_WALL_0;
        } else
        if (i == OptionsMain::CHECKPOINTS && !Options::checkpoints) {
            color = COLOR_WALL_0;
        } else
        if (i == OptionsMain::CONTROLLER_DIRECTION && !Options::controller_direction) {
            color = COLOR_WALL_0;
        } else
        if (i == OptionsMain::SPACER0 || i == OptionsMain::SPACER1 || i == OptionsMain::SPACER2 || 
            i == OptionsMain::SPACER3 || i == OptionsMain::SPACER4 || i == OptionsMain::SPACER5) {
            color = COLOR_ITEM_COLOR_0;
        }
        Panel::drawLine(pos, size, color, false);
    }
}

// # ================================================================ #
// # ::                                                            :: #
// # ================================================================ #
