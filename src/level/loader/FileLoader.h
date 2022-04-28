#pragma once

#include <stdint.h>

#include "../../Settings.h"

class FileLoader {

public:
    static bool load(uint8_t floor, uint8_t room);

private:
#if ENABLE_FILE_LOADER

    static bool process(uint8_t &pos, char* buffer);

    static bool match(uint8_t &pos, char* buffer, const char *token);

    static uint8_t readInt(uint8_t &pos, char* buffer);

    static void intToDec(uint8_t value, char* text);

    static int32_t decToInt(uint8_t &pos, char* buffer);

#endif

};
