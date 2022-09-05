#include "Platform.h"

#include "Pin.h"

// # ================================================================ #
// # ::  PLATFORM                                                  :: #
// # ================================================================ #

void platform_init(int argc, const char **argv) {
    Pin::setModeDigitalRead<OPTIONS_PIN>();
    Pin::writeHigh<OPTIONS_PIN>();

    Pin::setModeDigitalRead<ATTACK_PIN>();
    Pin::writeHigh<ATTACK_PIN>();
}

void platform_deinit() {

}

void platform_update() {

}

void platform_poll() {

}

uint8_t platform_read_byte_progmem(const uint8_t* ptr) {
    return pgm_read_byte_near(ptr);
}

char platform_read_byte_progmem(const char* ptr) {
    return pgm_read_byte_near(ptr);
}

// # ================================================================ #
// # ::  TIMING                                                    :: #
// # ================================================================ #

uint32_t platform_millis() {
    return millis();
}

void platform_watchdog_enable(int timing) {
    wdt_enable(timing);    
}

void platform_watchdog_disable() {
    wdt_disable();
}

void platform_watchdog_reset() {
    wdt_reset();
}

// # ================================================================ #
// # ::  LED STRIP                                                 :: #
// # ================================================================ #

#include "WS2811.h"

WS2811 leds;

void leds_init() {
    leds.begin();
}

void leds_reset() {
    leds.reset();
}

void leds_show(uint8_t *indices, uint8_t *palette, uint8_t index_bits, uint16_t led_count) {
    leds.show((WS2811::Color*) palette, indices, index_bits, led_count);
}

// # ================================================================ #
// # ::  CONTROLLER                                                :: #
// # ================================================================ #

#ifdef SIMPLE_CONTROLLER

int8_t angle_buffer[2];

uint8_t joystick_min;
uint8_t joystick_mid;
uint8_t joystick_max;

void controller_init() {

}

void controller_callibrate() {
    uint16_t start = platform_millis();
    joystick_min = 255;
    joystick_max = 0;
    while (platform_millis() - start < 2000) {
        int16_t ax = Pin::readAnalog(SIMPLE_CONTROLLER_PIN_MOVE) >> 2;
        if (ax > joystick_max) {
            joystick_max = ax;
        } else if (ax < joystick_min) {
            joystick_min = ax;
        }
        joystick_mid = ax;
    }
}

int8_t controller_poll_angle() {
    /**
     * Move:
     *   read the analog value of the move pin
     */
    int16_t ax = Pin::readAnalog(SIMPLE_CONTROLLER_PIN_MOVE) >> 2;

    if (ax < joystick_min) {
        ax = joystick_min;
    } else if (ax > joystick_max) {
        ax = joystick_max;
    }
    ax -= joystick_mid;
    if (ax < -SIMPLE_CONTROLLER_DEADZONE) {
        ax += SIMPLE_CONTROLLER_DEADZONE;
        ax = (ax * 100) / (joystick_mid - joystick_min);
    } else if (ax > SIMPLE_CONTROLLER_DEADZONE) {
        ax -= SIMPLE_CONTROLLER_DEADZONE;
        ax = (ax * 100) / (joystick_max - joystick_mid);
    } else {
        ax = 0;
    }*/

    /*
     * the potentiometer I used was fairly consistent, i.e. when not moving,
     * the reported value remained almost constant.
     * in such a case, the following code can be left out.
     * It smoothes the value by applying a 3 wide median filter.
     *
     * on the other hand, the code requires almost no memory at all, so one
     * can just include it.
     */
    // find median angle
    int8_t median_angle = (int8_t) ax;
    if (angle_buffer[0] < median_angle) {
        if (angle_buffer[1] < median_angle) {
            median_angle = angle_buffer[0] > angle_buffer[1] ? angle_buffer[0] : angle_buffer[1];
        }
    } else if (angle_buffer[1] > median_angle) {
        median_angle = angle_buffer[0] < angle_buffer[1] ? angle_buffer[0] : angle_buffer[1];
    }

    // add new sample to angle_buffer
    angle_buffer[1] = angle_buffer[0];
    angle_buffer[0] = ax;
    return ax;
}

#endif

bool controller_poll_attack() {
    return !Pin::read<ATTACK_PIN>();
}

bool controller_poll_options() {
    return !Pin::read<OPTIONS_PIN>();
}

bool controller_poll_turn_off() {
    return false;
}

// # ================================================================ #
// # ::  SOUND                                                     :: #
// # ================================================================ #

#if ENABLE_SOUND

void sound_init() {
    
}

#endif

// # ================================================================ #
// # ::  LOG                                                       :: #
// # ================================================================ #

#if ENABLE_LOGGING

void log_init() {
    Serial.begin(BAUDRATE);
}

void log(const char *text) {
    Serial.print(text);
}

void log(int64_t val) {
    Serial.print(val);
}

void logln(const char *text) {
    Serial.println(text);
}

void logln(int64_t val) {
    Serial.println(val);
}

#endif

// # ================================================================ #
// # ::  RUNTIME SETTINGS                                          :: #
// # ================================================================ #

#if ENABLE_RUNTIME_SETTINGS

#include <EEPROM.h>

void settings_init() {

}

void settings_default() {
#ifdef SIMPLE_CONTROLLER
    joystick_min = DEFAULT_JOYSTICK_MIN;
    joystick_mid = DEFAULT_JOYSTICK_MID;
    joystick_max = DEFAULT_JOYSTICK_MAX;
#endif
}

void settings_check() {
#ifdef SIMPLE_CONTROLLER
    if (joystick_min >= joystick_mid || joystick_mid >= joystick_max) {
        joystick_min = DEFAULT_JOYSTICK_MIN;
        joystick_mid = DEFAULT_JOYSTICK_MID;
        joystick_max = DEFAULT_JOYSTICK_MAX;
    }
#endif
}

void settings_write(void *data, uint16_t size) {
    uint8_t *ptr = (uint8_t*) data;
    for (uint16_t i = 0; i < size; i++) {
        EEPROM.update(i, ptr[i]);
    }
}

void settings_read(void *data, uint16_t size) {
    uint8_t *ptr = (uint8_t*) data;
    for (uint16_t i = 0; i < size; i++) {
        ptr[i] = EEPROM.read(i);
    }
}

void settings_write_platform_specific(uint16_t offset) {
#ifdef SIMPLE_CONTROLLER
    EEPROM.update(offset++, joystick_min);
    EEPROM.update(offset++, joystick_mid);
    EEPROM.update(offset, joystick_max);
#endif
}

void settings_read_platform_specific(uint16_t offset) {
#ifdef SIMPLE_CONTROLLER
    joystick_min = EEPROM.read(offset++);
    joystick_mid = EEPROM.read(offset++);
    joystick_max = EEPROM.read(offset);
#endif
}

#endif

// # ================================================================ #
// # ::  FILE SYSTEM                                               :: #
// # ================================================================ #

#if ENABLE_FILE_LOADER

#include <SdFat.h>
#include <SPI.h>

SdFat32 SD;
FatFile file;

bool file_system_init() {
    return SD.begin(4);
}

void file_system_deinit() {
    SD.end();
}

bool file_open(const char *path) {
    file = SD.open(path);
    return file;
}

void file_close() {
    file.close();
}

bool file_has_next_char() {
    return file.available();
}

char file_get_next_char() {
    return file.read();
}

#endif

// # ================================================================ #
// # ::                                                            :: #
// # ================================================================ #
