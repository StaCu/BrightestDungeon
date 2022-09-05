#include "Random.h"

// for more information, see
// https://en.wikipedia.org/wiki/Linear_congruential_generator 

uint32_t Random::seed = 9815683; // start value 0 <= seed < m

void Random::setSeed(uint32_t seed) {
    Random::seed = seed;
}

uint16_t Random::get() {
    const uint32_t a = 1103515245; // multiplier 0 < a < m
    const uint32_t c = 12345;      // increment  0 <= c < m
    const uint32_t m = 0x7fffffff; // modulus    0 < m
    seed = (a * seed + c) & m;
    return seed >> 15;
}

uint16_t Random::get(uint16_t max_plus_one) {
    return get() % max_plus_one;
}
