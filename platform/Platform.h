#pragma once

// # ================================================================ #
// # ::  INCLUDE                                                   :: #
// # ================================================================ #

#include <stdint.h>

// # ================================================================ #
// # ::  MISC                                                      :: #
// # ================================================================ #

#define MAX_LIVES              5
#define MAX_ENTITY_COUNT       42

// # ================================================================ #
// # ::  PLATFORM                                                  :: #
// # ================================================================ #

#ifndef F
#define F(x) x
#endif

#ifndef PROGMEM
#define PROGMEM
#endif

#define ENTRY_POINT(x) \
int main() { \
    platform_init(argc, argv); \
    {x;} \
    return 0; \
}

void platform_init(int argc, const char **argv);

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

#define COLOR_SWIZZLE(R,G,B) R,G,B

// the 2 most useful values for COLOR_PALETTE_INDEX_BITS are 5 and 8
// 8: each index is 8bit wide, making it really easy to iterate the indices-array in leds_show(...).
// 5: each index is 5bit wide, which is the minimum necessary to cover all colors in the game.
//    This saves some memory, however, reading from the array becomes more difficult as indices may
//    not start on a byte boundary or even cover two bytes.
// => choose 8 unless the platform is memory constrained.
#define COLOR_PALETTE_INDEX_BITS 5

void leds_init();

void leds_reset();

void leds_show(uint8_t *indices, uint8_t *palette, uint8_t index_bits, uint16_t led_count);

// # ================================================================ #
// # ::  CONTROLLER                                                :: #
// # ================================================================ #

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

#define ENABLE_LOGGING false
#define BAUDRATE 115200
#define LOG_CONTROLLER false
#define LOG_TIMING_SLACK false

void platform_log_init();

void platform_log(const char *text);

void platform_log(int64_t val);

void platform_logln(const char *text);

void platform_logln(int64_t val);

#if ENABLE_LOGGING
#define LOG(x) platform_log(x)
#define LOG_LN(x) platform_logln(x)
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
