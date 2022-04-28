#include "Simulator.h"

#ifdef SIMULATOR

#include "../Settings.h"
#include <chrono>
#include <filesystem>
#include <string>
#include <cstring>
#include <istream>
#include <ostream>
#include <fstream>

uint8_t pgm_read_byte_near(const uint8_t *ptr) {
    return *ptr;
}

char pgm_read_byte_near(const char *ptr) {
    return *ptr;
}

void delay(int x) {

}

auto START_TIME = std::chrono::system_clock::now().time_since_epoch();

uint32_t millis() {
    auto NOT_TIME = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(NOT_TIME-START_TIME).count();
}

void wdt_enable(int x) {

}

void wdt_disable() {

}

void wdt_reset() {

}

std::filesystem::path simulator_path = "";
std::filesystem::path base_path = "";
std::filesystem::path eeprom_path = "eeprom.bin";
std::filesystem::path log_path = "";

std::ofstream *log_file;

void setSimulatorPath(const char *path) {
    base_path = path;
    simulator_path = base_path / "dng"; 
    std::filesystem::create_directories(simulator_path);
    eeprom_path = simulator_path / "eeprom.bin";
    log_path = simulator_path / "log.txt";

    #if ENABLE_LOGGING
    log_file = new std::ofstream(log_path.c_str());
    #endif
}

void EEPROMstruct::write(uint16_t addr, uint8_t value) {
    addr &= 0b0000001111111111;
    uint8_t *buffer = new uint8_t[1024];

    if (std::filesystem::exists(eeprom_path)) {
        std::ifstream is(eeprom_path.c_str(), std::ios::binary);
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);
        int count = length;
        if (count > 1024) {
            count = 1024;
        }
        is.read((char*)buffer,count);
        is.close();
    } else {
        memset(buffer, 0, 1024);
    }

    buffer[addr] = value;

    std::ofstream os(eeprom_path.c_str(), std::ios::binary);
    os.write((char*)buffer, 1024);
    os.close();
    delete[] buffer;
}

void EEPROMstruct::read(uint16_t addr, uint8_t &value) {
    addr &= 0b0000001111111111;
    uint8_t *buffer = new uint8_t[1024];

    if (std::filesystem::exists(eeprom_path)) {
        std::ifstream is(eeprom_path.c_str(), std::ios::binary);
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);
        int count = length;
        if (count > 1024) {
            count = 1024;
        }
        is.read((char*)buffer,count);
        is.close();
    } else {
        memset(buffer, 0, 1024);
    }

    value = buffer[addr];
    delete[] buffer;
}

EEPROMstruct EEPROM;

FatFile::FatFile() {
    buffer = nullptr;
}

FatFile::FatFile(const char *path) 
: path(path) {
    idx = 0;
    std::filesystem::path file_path = base_path / path;
    if (std::filesystem::exists(file_path)) {
        std::ifstream is(file_path.c_str(), std::ios::binary);
        is.seekg (0, is.end);
        size = is.tellg();
        is.seekg (0, is.beg);
        buffer = new uint8_t[size];
        is.read((char*)buffer,size);
        is.close();
    } else {
        buffer = nullptr;
        size = 0;
    }
}

bool FatFile::available() {
    return idx < size;
}

char FatFile::read() {
    char c = buffer[idx];
    idx++;
    return c;
}

void FatFile::close() {
    delete[] buffer;
}

FatFile::operator bool() {
    return buffer;
}

uint32_t last_log = 0;

void FatFile::log(const char *text) {
    #if ENABLE_LOGGING
    *log_file << text;
    #ifdef INSTANT_FLUSH
        log_file->flush();
    #endif
    #endif
}

void FatFile::log(std::string text) {
    #if ENABLE_LOGGING
    *log_file << text;
    #ifdef INSTANT_FLUSH
        log_file->flush();
    #endif
    #endif
}

bool SdFat32::begin(uint8_t pin) {
    return true;
}

void SdFat32::end() {

}

FatFile SdFat32::open(const char *path) {
    return FatFile(path);
}

void Simulator::init(int argc, const char **argv) {
    if (argc > 1) {
        setSimulatorPath(argv[1]);
    } else {
        setSimulatorPath("");
    }

    initscr();
    noecho();
    cbreak();
    timeout(-1);
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(FALSE);
}

int ncurses_character = 0;

void Simulator::update() {

    refresh();
    move(0,0);
    clear();

    #if ENABLE_LOGGING
    if (last_log - millis() > 1000) {
        log_file->flush();
        last_log = millis();
    }
    #endif

}

bool Simulator::poll() {
    ncurses_character = getch();
    return ncurses_character == 'q';
}

void Simulator::destroy() {
    endwin();

    #if ENABLE_LOGGING
    log_file->close();
    delete log_file;
    #endif
}

int Simulator::getButton() {
    return ncurses_character;
}

#endif