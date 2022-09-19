#pragma once

#include <stdint.h>

class ExplosionAnimation {

private:
    enum State : uint8_t {
        OFF = 0,
        LOOSE = 1,
        EXPLODE = 21,
        IMPLODE = EXPLODE + 1,
        DOOR = IMPLODE + 1
    };

    static uint8_t show_idx;
    static uint8_t color;
    static uint8_t state;
    static int8_t substep;
    static int16_t pos;
    static int16_t size;
    static int16_t maxsize;

public:

    static void drawFloorIndex(int16_t min, int16_t max);

    static void explode(uint8_t color, bool show_idx = true);

    static void implode(uint8_t color);

    static void door(uint8_t color, int16_t pos);

    static void update();

    static void draw();

    static bool isActive();

    static bool isExplosion();

    static bool isImplosionOrDoor();
    
};
