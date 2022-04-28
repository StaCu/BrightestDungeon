#pragma once

#ifdef SIMULATOR

#include "simulator/Simulator.h"

#else

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <stdint.h>

#endif

// # ================================================================ #
// # ::  SETTINGS                                                  :: #
// # ================================================================ #

// # ================================ #
// # ::  CONTROLLER                :: #
// # ================================ #

#ifndef SIMULATOR
#define USE_MPU6050_CONTROLLER
// PIN Analog 4, 5
#define MPU6050_CONTROLLER_ORIENTATION  0     // 0, 1 or 2 to set the angle of the joystick
#define MPU6050_CONTROLLER_DEADZONE     8     // Angle to ignore
#endif

#ifndef USE_MPU6050_CONTROLLER
#define USE_SIMPLE_CONTROLLER
// PIN Analog 5
#define SIMPLE_CONTROLLER_PIN_MOVE     5
#define SIMPLE_CONTROLLER_DEADZONE     8     // Angle to ignore
#endif

// PIN Digital 2
#define ATTACK_PIN PORTD, 2

// # ================================ #
// # ::  LED                       :: #
// # ================================ #

// drawing the leds alone takes "LED_COUNT * 24 bits per led * 20 cycle per bit / 16000 cycles per second" seconds
// at 512 this results in 15.36ms leaving only 4.64ms for the rest of the game
// => using the full 512 leds will make the game feel less fluent unless the UPDATE time is increased as well
#define LED_STRIP_COUNT 300
#define LED_MIN_COUNT 100
#define LED_MAX_BRIGHTNESS 5

// number of status leds
#define LED_STATUS_COUNT 0

// PIN 8 (hardcoded)
#define LED_PIN         8

#define LED_MAX_COUNT (LED_STRIP_COUNT + LED_STATUS_COUNT)

#define LED_COLOR_CORRECT_R 255
#define LED_COLOR_CORRECT_G 255
#define LED_COLOR_CORRECT_B 255

#define COLOR_PALETTE 0

// # ================================ #
// # ::  SOUND                     :: #
// # ================================ #

#define ENABLE_SOUND true
// PINS 9 & 10 (hardcoded in toneAC)
#define SOUND_MAX_VOLUME 4

// # ================================ #
// # ::  SDCARD                    :: #
// # ================================ #

#define ENABLE_INTERNAL_DUNGEON true
#define SHORTEN_INTERNAL_DUNGEON false

#define ENABLE_FILE_LOADER true

// max buffer size is 254, because 255 is used to identify errors
#define FILEIO_BUFFER_SIZE 64

// the directory and file names are hardcoded
// the N is replaced by a digit from 0-f
#define FILEIO_DUNGEON_DIRECTORY "dng/N/"
// the FFF by a number from 001-255
#define FILEIO_FLOOR_DIRECTORY "FFF/"
// the RRR by a number from 000-255
#define FILEIO_ROOM_FILE "RRR.txt"

#define LEVEL_DESCRIPTION_MAX_PARAMETER_PER_UNIT 5

// # ================================ #
// # ::  OPTIONS                   :: #
// # ================================ #

#define USE_EEPROM true

#define OPTIONS_PIN PORTD, 3

#define MAX_LIVES              5
#define UPDATE_TIME            20
#define POOL_SIZE              42
#define STAND_BY_TIMEOUT       64000
#define STAND_BY_TIMEOUT_RESET 74000

#define DEFAULT_TESTVALUE      1
#define DEFAULT_DUNGEON        0
#define DEFAULT_FIRST_FLOOR    0
#define DEFAULT_CHECKPOINTS    1

#define DEFAULT_LED_COUNT      144
#define DEFAULT_BRIGHTNESS     1
#define DEFAULT_VOLUME         1

#define DEFAULT_DIRECTION      1
#define DEFAULT_JOYSTICK_MIN   0
#define DEFAULT_JOYSTICK_MID   127
#define DEFAULT_JOYSTICK_MAX   255

// # ================================ #
// # ::  DEGUB                     :: #
// # ================================ #

#define ENABLE_LOGGING false
#define BAUDRATE 115200
#define LOG_CONTROLLER false
#define LOG_TIMING_SLACK false

#ifndef SIMULATOR
#if ENABLE_LOGGING
#define LOG(x) Serial.print(x)
#define LOG_LN(x) Serial.println(x)
#else
#define LOG(x)
#define LOG_LN(x)
#endif

#define ENABLE_WATCHDOG       false      // using a wdt resulted in problems
                                         // when reprogramming, so it is disabled
                                         // by default
#define WATCHDOG_INTERVAL     WDTO_120MS // 15,30,60,120,250,500,1000,2000,4000,8000
#endif

// # ================================================================ #
// # ::                                                            :: #
// # ================================================================ #
