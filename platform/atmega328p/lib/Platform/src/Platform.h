#pragma once

// # ================================================================ #
// # ::  INCLUDE                                                   :: #
// # ================================================================ #

#if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <stdint.h>

// # ================================================================ #
// # ::  MISC                                                      :: #
// # ================================================================ #

#define MAX_LIVES              5
#define MAX_ENTITY_COUNT       42

// # ================================================================ #
// # ::  PLATFORM                                                  :: #
// # ================================================================ #

#define ENTRY_POINT(x) \
int main() { \
    platform_init(); \
    {x;} \
    return 0; \
}

void platform_init();

void platform_deinit();

void platform_update();

void platform_poll();

uint8_t platform_read_byte_progmem(const uint8_t *ptr);

char platform_read_byte_progmem(const char *ptr);

// # ================================================================ #
// # ::  TIMING                                                    :: #
// # ================================================================ #

#define STARTUP_SAFETY_TIME    1000

#define UPDATE_TIME            20
#define STAND_BY_TIMEOUT       64000
#define STAND_BY_TIMEOUT_RESET 74000

#define ENABLE_WATCHDOG       false
#define WATCHDOG_INTERVAL     WDTO_120MS

uint32_t platform_millis();

void platform_watchdog_enable();

void platform_watchdog_disable();

void platform_watchdog_reset();

// # ================================================================ #
// # ::  LED STRIP                                                 :: #
// # ================================================================ #

#define LED_STRIP_COUNT 300
#define LED_STATUS_COUNT 0

#define LED_MIN_COUNT 100
#define LED_MAX_COUNT (LED_STRIP_COUNT + LED_STATUS_COUNT)

#define LED_MAX_BRIGHTNESS 5

#define LED_COLOR_CORRECT_R 255
#define LED_COLOR_CORRECT_G 255
#define LED_COLOR_CORRECT_B 255

//#define USE_HIGHER_CONTRAST_COLOR_PALETTE

// PIN 8 (hardcoded)
#define LED_PIN         8

#define COLOR_SWIZZLE(R,G,B) G,R,B

#define COLOR_PALETTE_INDEX_BITS 5

void leds_init();

void leds_reset();

void leds_show(uint8_t *indices, uint8_t *palette, uint8_t index_bits, uint16_t led_count);

// # ================================================================ #
// # ::  CONTROLLER                                                :: #
// # ================================================================ #

//#define MPU6050_CONTROLLER
// PIN Analog 4, 5
#define MPU6050_CONTROLLER_ORIENTATION  0     // 0, 1 or 2 to set the angle of the joystick
#define MPU6050_CONTROLLER_DEADZONE     8     // Angle to ignore

#ifndef MPU6050_CONTROLLER
#define SIMPLE_CONTROLLER
// PIN Analog 5
#define SIMPLE_CONTROLLER_PIN_MOVE     5
#define SIMPLE_CONTROLLER_DEADZONE     8     // Angle to ignore
#endif

// PIN Digital 2
#define ATTACK_PIN AVR_PORTD, 2

#define OPTIONS_PIN AVR_PORTD, 3

#define DEFAULT_JOYSTICK_MIN   0
#define DEFAULT_JOYSTICK_MID   127
#define DEFAULT_JOYSTICK_MAX   255

void controller_init();

void controller_callibrate();

int8_t controller_poll_angle();

bool controller_poll_attack();

bool controller_poll_options();

bool controller_poll_turn_off();

// # ================================================================ #
// # ::  SOUND                                                     :: #
// # ================================================================ #

#define ENABLE_SOUND true
#define SOUND_MAX_VOLUME 4

// PINS 9 & 10 (hardcoded in toneAC)

void sound_init();

/**
 * configure the PWM hardware to generate a continous output with a given
 * frequency and volume
 * 
 * if (frequency == 0 || volume == 0)
 *      this will turn the PWM off
 * 
 * @param frequency 
 *      the frequency
 * @param volume 
 *      the volume [0..4]
 */
void sound_play(uint16_t frequency, uint8_t volume);

// # ================================================================ #
// # ::  LOG                                                       :: #
// # ================================================================ #

#define ENABLE_LOGGING true
#define BAUDRATE 115200
#define LOG_CONTROLLER false
#define LOG_TIMING_SLACK false

void platform_log_init();

void platform_log(const char *text);

void platform_log(int64_t val);

void platform_logln(const char *text);

void platform_logln(int64_t val);

#if ENABLE_LOGGING
#define LOG(x) Serial.print(x) //platform_log(x)
#define LOG_LN(x) Serial.println(x) //platform_logln(x)
#else
#define LOG(x)
#define LOG_LN(x)
#endif

// # ================================================================ #
// # ::  RUNTIME SETTINGS                                          :: #
// # ================================================================ #

#define ENABLE_RUNTIME_SETTINGS true

#define DEFAULT_TESTVALUE      1
#define DEFAULT_DUNGEON        0
#define DEFAULT_FIRST_FLOOR    0
#define DEFAULT_CHECKPOINTS    1

#define DEFAULT_LED_COUNT      144
#define DEFAULT_BRIGHTNESS     1
#define DEFAULT_VOLUME         1

#define DEFAULT_DIRECTION      1

void settings_init();

void settings_default();

void settings_check();

void settings_write(void *data, uint16_t size);

void settings_read(void *data, uint16_t size);

void settings_write_platform_specific(uint16_t offset);

void settings_read_platform_specific(uint16_t offset);

// # ================================================================ #
// # ::  FILE SYSTEM                                               :: #
// # ================================================================ #

#define ENABLE_FILE_LOADER true

#define ENABLE_INTERNAL_DUNGEON true

bool file_system_init();

void file_system_deinit();

bool file_open(const char *path);

void file_close();

bool file_has_next_char();

char file_get_next_char();

// # ================================================================ #
// # ::                                                            :: #
// # ================================================================ #
