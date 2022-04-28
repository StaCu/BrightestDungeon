#pragma once

#ifdef SIMULATOR

#include <ncurses.h>
#include <stdint.h>
#include <iostream>

#define WDTO_120MS 0
#define PROGMEM

#define SHOW_SOUND true

//#define INSTANT_FLUSH

#define F(x) x

#define LOG(x) FatFile::log(x);
#define LOG_LN(x) FatFile::log(x); FatFile::log("\n");

template<typename T>
T abs(T a) {
    return a > 0 ? a : -a;
}

uint8_t pgm_read_byte_near(const uint8_t *ptr);

char pgm_read_byte_near(const char *ptr);

void delay(int x);

uint16_t random(int x);
uint32_t millis();

void wdt_enable(int x);

void wdt_disable();

void wdt_reset();

void setSimulatorPath(const char *path);

struct EEPROMstruct {
private:

    void write(uint16_t addr, uint8_t value);
    void read(uint16_t addr, uint8_t &value);

public:

    template<typename Type>
    void get(uint16_t addr, Type &value) {
        uint8_t *ptr = (uint8_t*) &value;
        for (int i = 0; i < sizeof(Type); i++) {
            read(addr+i, *ptr);
        }
    }

    template<typename Type>
    void put(uint16_t addr, Type value) {
        uint8_t *ptr = (uint8_t*) &value;
        for (int i = 0; i < sizeof(Type); i++) {
            write(addr+i, *ptr);
        }
    }

};

extern EEPROMstruct EEPROM;

struct FatFile {
private:
    const char *path;
    
    uint8_t *buffer;
    int size;
    int idx;

public:
    FatFile();
    FatFile(const char *path);

    bool available();
    char read();
    void close();

    static void log(const char *text);
    static void log(std::string text);
    static void log(int64_t val) {
        log(std::to_string(val));
    }

    operator bool();

};

struct SdFat32 {
public:
    bool begin(uint8_t pin);
    void end();
    FatFile open(const char *path);

};

class Simulator {
public:
    static void init(int argc, const char **argv);
    static void update();
    static bool poll();
    static void destroy();

    static int getButton();

};


#endif
