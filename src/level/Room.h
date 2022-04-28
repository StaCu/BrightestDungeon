#pragma once

#include <stdint.h>

class Room {

public:
    static void update();

    static void draw();

    static bool load(uint8_t special);

};
