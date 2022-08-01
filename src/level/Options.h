#pragma once

#include <stdint.h>

class Options {
public:

    // # ================================================================ #
    // # ::  OPTIONS                                                   :: #
    // # ================================================================ #
    
    static uint8_t dungeon;
    static uint8_t start_floor;
    static bool checkpoints;

    static int16_t led_count;
    static uint8_t brightness;
    static uint8_t volume;

    static uint8_t controler_direction;
    static uint8_t joystick_min;
    static uint8_t joystick_mid;
    static uint8_t joystick_max;

    static bool swap_color;
    static int16_t level_length;
    static int8_t sub_steps;
    static int8_t size_large;
    static int8_t size_medium;
    static int8_t size_small;

    static void read();

    static void write();
    
    static int16_t scalePosition(uint8_t pos);

    static void updateLevelLength();

    static uint16_t getRandom();

    static uint16_t getRandom(uint16_t x);

    // # ================================================================ #
    // # ::  SELECTION                                                 :: #
    // # ================================================================ #

private:
    enum State : uint8_t {
        STATE_OFF,
        STATE_IDLE,
        STATE_MAIN
    };
    
    static uint8_t use;
    static State state;

    static uint8_t tmp_dungeon;
    static uint8_t tmp_floor;

    static uint8_t getSelection();

public:
    static void select();

    static void stopCombo();

    // # ================================================================ #
    // # ::  STATE                                                     :: #
    // # ================================================================ #

    static void setIdle();

    static bool isIdle();

    static void setOptions();
    
    static bool isOptions();

    static void off();

    // # ================================================================ #
    // # ::  UPDATE                                                    :: #
    // # ================================================================ #

    static void update();

private:
    enum OptionsMain : uint8_t {
        CALLIBRATE,
        CONTROLLER_DIRECTION,

        SPACER0,

        DECR_LED_COUNT,
        INCR_LED_COUNT,
        DECR_BRIGHTNESS,
        INCR_BRIGHTNESS,
        DECR_VOLUME,
        INCR_VOLUME,

        SPACER1,

        CHECKPOINTS,

        SPACER2,

        DUNGEON0,
        DUNGEON1,
        DUNGEON2,
        DUNGEON3,

        SPACER3,

        FLOOR0,
        FLOOR1,
        FLOOR2,
        FLOOR3,
        FLOOR4,
        FLOOR5,
        FLOOR6,
        FLOOR7,

        SPACER4,

        READ_OPTIONS,
        WRITE_OPTIONS,

        SPACER5
    };

    static void setHeroPosition(uint8_t slot);

    // # ================================================================ #
    // # ::  DRAW                                                      :: #
    // # ================================================================ #

public:
    static void draw();

    // # ================================================================ #
    // # ::                                                            :: #
    // # ================================================================ #

};
