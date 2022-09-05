#include "Math.h"

uint8_t math::abs8(int8_t val) {
    return val < 0 ? -val : val;
}

uint16_t math::abs16(int16_t val) {
    return val < 0 ? -val : val;
}
