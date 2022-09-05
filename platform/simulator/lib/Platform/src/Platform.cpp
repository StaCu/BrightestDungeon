#include "Platform.h"

#include <ncurses.h>
#include <chrono>
#include <stdint.h>
#include <filesystem>
#include <string>
#include <cstring>
#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>

// # ================================================================ #
// # ::  PLATFORM                                                  :: #
// # ================================================================ #

int ncurses_character = 0;
auto START_TIME = std::chrono::system_clock::now().time_since_epoch();

std::filesystem::path simulator_path = "";
std::filesystem::path base_path = "";
std::filesystem::path eeprom_path = "eeprom.bin";
std::filesystem::path log_path = "";

int last_log;
std::ofstream *log_file;

int sound_freq = 0;
int sound_vol = 0;

void platform_init(int argc, const char **argv) {
    const char *path = "";
    if (argc > 1) {
        path = argv[1];
    }

    base_path = path;
    simulator_path = base_path / "dng"; 
    std::filesystem::create_directories(simulator_path);
    eeprom_path = simulator_path / "eeprom.bin";
    log_path = simulator_path / "log.txt";

    #if ENABLE_LOGGING
    log_file = new std::ofstream(log_path.c_str());
    last_log = platform_millis();
    #endif

    initscr();
    noecho();
    cbreak();
    timeout(-1);
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(FALSE);
}

void platform_deinit() {
    endwin();
    #if ENABLE_LOGGING
    log_file->close();
    delete log_file;
    #endif
}

void platform_update() {
    refresh();
    move(0,0);
    clear();

    #if ENABLE_LOGGING
    if (last_log - platform_millis() > 1000) {
        log_file->flush();
        last_log = platform_millis();
    }
    #endif
}

void platform_poll() {
    ncurses_character = getch();
}

uint8_t platform_read_byte_progmem(const uint8_t *ptr) {
    return *ptr;
}

char platform_read_byte_progmem(const char *ptr) {
    return *ptr;
}

// # ================================================================ #
// # ::  TIMING                                                    :: #
// # ================================================================ #

uint32_t platform_millis() {
    auto NOW_TIME = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(NOW_TIME-START_TIME).count();
}

void platform_watchdog_enable(int timing) {
    
}

void platform_watchdog_disable() {

}

void platform_watchdog_reset() {

}

// # ================================================================ #
// # ::  LED STRIP                                                 :: #
// # ================================================================ #

void leds_init() {

}

void leds_reset() {

}

void leds_show(uint8_t *indices, uint8_t *palette, uint8_t index_bits, uint16_t led_count) {
    uint8_t colors[]     = {  0, 37, 94, 36, 36, 33, 33, 33, 33, 34, 94, 37, 37, 37, 36, 36, 32, 34, 90, 37, 31, 31, 91, 36, 32, 34};
    uint8_t characters[] = {' ','X','=','-','-','=','L','-','=','-','=','s','S','D','#','-','-','-','w','W','M','M','F','A','H','H'};

    std::string leds = "[";
    for (uint64_t i = 0; i < led_count; i++) {
        uint8_t val = indices[i];
        leds += "\033[1;";
        leds += std::to_string(colors[val]);
        leds += "m";
        leds += (char) characters[val];
        leds += "\033[0m";
    }
    leds += "]";
   // mvprintw(0, 0, leds.c_str());

    std::string tone = "tone(f: " + std::to_string((int) sound_freq) + ",v: " + std::to_string((int) sound_vol) + ")";
    std::cout << leds << " " << tone << std::endl;
}

// # ================================================================ #
// # ::  CONTROLLER                                                :: #
// # ================================================================ #

void controller_init() {

}

void controller_callibrate() {
    
}

int8_t controller_poll_angle() {
    if (ncurses_character == CONTROLLER_GO_LEFT) return 90;
    if (ncurses_character == CONTROLLER_GO_RIGHT) return -90;
    return 0;
}

bool controller_poll_attack() {
    return ncurses_character == CONTROLLER_ATTACK;
}

bool controller_poll_options() {
    return ncurses_character == CONTROLLER_OPTIONS;    
}

bool controller_poll_turn_off() {
    return ncurses_character == CONTROLLER_QUIT;
}

// # ================================================================ #
// # ::  SOUND                                                     :: #
// # ================================================================ #

#if ENABLE_SOUND

void sound_init() {
    
}

void sound_play(uint16_t frequency, uint8_t volume) {
    if (volume != 255) {
        sound_freq = frequency;
        sound_vol = volume;
    }

    //std::string text = "tone(f: " + std::to_string((int) sound_freq) + ",v: " + std::to_string((int) sound_vol) + ")";
    //mvprintw(1, 0, text.c_str());
    //std::cout << text << std::endl;
}

#endif

// # ================================================================ #
// # ::  LOG                                                       :: #
// # ================================================================ #

#if ENABLE_LOGGING

void log_init() {

}

void log(const char *text) {
    *log_file << text;
    #ifdef INSTANT_FLUSH
        log_file->flush();
    #endif
}

void log(int64_t val) {
    *log_file << val;
    #ifdef INSTANT_FLUSH
        log_file->flush();
    #endif
}

void logln(const char *text) {
    *log_file << text << std::endl;
    #ifdef INSTANT_FLUSH
        log_file->flush();
    #endif
}

void logln(int64_t val) {
    *log_file << val << std::endl;
    #ifdef INSTANT_FLUSH
        log_file->flush();
    #endif
}

#endif

// # ================================================================ #
// # ::  RUNTIME SETTINGS                                          :: #
// # ================================================================ #

#if ENABLE_RUNTIME_SETTINGS

void settings_init() {

}

void settings_default() {

}

void settings_check() {

}

void settings_write(void *data, uint16_t size) {
    std::ofstream os(eeprom_path.c_str(), std::ios::binary);
    os.write((char*)data, size);
    os.close();
}

void settings_read(void *data, uint16_t size) {
    if (std::filesystem::exists(eeprom_path)) {
        std::ifstream is(eeprom_path.c_str(), std::ios::binary);
        is.read((char*)data,size);
        is.close();
    } else {
        memset(data, 0, size);
    }
}

void settings_write_platform_specific(uint16_t offset) {

}

void settings_read_platform_specific(uint16_t offset) {

}

#endif

// # ================================================================ #
// # ::  FILE SYSTEM                                               :: #
// # ================================================================ #

#if ENABLE_FILE_LOADER

uint8_t *file_buffer;
int file_size;
int file_idx;

bool file_system_init() {
    file_buffer = nullptr;
    file_size = 0;
    file_idx = 0;
    return true;
}

void file_system_deinit() {
    file_close();
}

bool file_open(const char *path) {
    file_idx = 0;
    std::filesystem::path file_path = base_path / path;
    if (std::filesystem::exists(file_path)) {
        std::ifstream is(file_path.c_str(), std::ios::binary);
        is.seekg (0, is.end);
        file_size = is.tellg();
        is.seekg (0, is.beg);
        file_buffer = new uint8_t[file_size];
        is.read((char*)file_buffer,file_size);
        is.close();
        return true;
    } else {
        file_buffer = nullptr;
        file_size = 0;
        return false;
    }
}

void file_close() {
    delete[] file_buffer;
    file_buffer = nullptr;
    file_size = 0;
    file_idx = 0;
}

bool file_has_next_char() {
    return file_idx < file_size;
}

char file_get_next_char() {
    char c = file_buffer[file_idx];
    file_idx++;
    return c;
}

#endif

// # ================================================================ #
// # ::                                                            :: #
// # ================================================================ #
