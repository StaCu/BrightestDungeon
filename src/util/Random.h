#pragma once

#include <stdint.h>

class Random {

private:

    static uint32_t seed;

public:

    static void setSeed(uint32_t seed);

    static uint16_t get();

    static uint16_t get(uint16_t max_plus_one);

};
