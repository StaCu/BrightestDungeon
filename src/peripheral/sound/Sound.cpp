#include "Sound.h"

#if ENABLE_SOUND

#include "toneAC.h"
#include "pitches.h"
#include "../../level/entity/environment/Fog.h"
#include "../../level/Options.h"

#include "../../Secret.h"

#define DIV4(x) ((x) / 4)

const uint8_t Sound::failure[] PROGMEM = {
    DIV4(NOTE_A4), 0, DIV4(NOTE_E4), 0, DIV4(NOTE_C4)
};

const uint8_t Sound::attack[] PROGMEM = {
    250, 248, 246, 244,
    242, 240, 238, 236,
    234, 232, 230, 228,
    226, 224, 222, 220
};

const uint8_t Sound::item[] PROGMEM = {
    DIV4(NOTE_A4), 0, DIV4(NOTE_C5)
};

const uint8_t Sound::success[] PROGMEM = {
    DIV4(NOTE_C4), 0, DIV4(NOTE_E4), 0, DIV4(NOTE_A4)
};

const uint8_t Sound::teleport[] PROGMEM = {
    DIV4(NOTE_A4), 0, DIV4(NOTE_A4), 0, DIV4(NOTE_A4)
};

const uint8_t Sound::victory[] PROGMEM = {
    DIV4(NOTE_C4), 0, DIV4(NOTE_E4), 0, DIV4(NOTE_C4), 0,
    DIV4(NOTE_A4), 0, DIV4(NOTE_E4), 0, DIV4(NOTE_A4), 0,
    DIV4(NOTE_C5), DIV4(NOTE_C5)
};

uint8_t Sound::type;
uint8_t Sound::idx;
int8_t Sound::arg;
uint8_t Sound::duration;
uint8_t Sound::bit;

void Sound::init() {
    type = Type::SILENT;
}

void Sound::update() {
    duration--;
    if (duration == 0) {
        uint16_t freq = 0;
        switch (type) {
            case Type::VICTORY : freq = playProgmem(victory, sizeof(victory), 8); break;
            case Type::SUCCESS : freq = playProgmem(success, sizeof(success), 4); break;
            case Type::FAILURE : freq = playProgmem(failure, sizeof(failure), 4); break;
            case Type::SECRET  : freq = playSecret(); break;
            case Type::TELEPORT: freq = playProgmem(teleport, sizeof(Sound::teleport), 4); break;
            case Type::ITEM    : freq = playProgmem(item, sizeof(item), 4); break;
            case Type::ATTACK  : freq = playProgmem(attack, sizeof(attack), 1); break;
            default: break;
        }
        toneAC(freq, Options::volume);
    } else {
#ifdef SIMULATOR
        toneAC(0, 255);
#endif
    }
}

uint16_t Sound::playProgmem(const uint8_t *ptr, uint8_t count, uint8_t duration) {
    if (idx == count) {
        type = Type::SILENT;
        return 0;
    }
    Sound::duration = duration;
    uint8_t value = pgm_read_byte_near(&ptr[idx]);
    idx++;
    return value * 4;
}

#define MORSE_TIME_UNIT 8

uint16_t Sound::playSecret() {
    Entity::global_mask &= ~0b00000001;
    arg -= 1;
    if (arg & 0b00000001) {
        // wait one dit time
        duration = MORSE_TIME_UNIT;
        return 0;
    }
    
    if (arg == 0) {
        bit = pgm_read_byte_near(&secret_code[idx]);
        idx += 1;
        arg  = 8;
        if (idx == sizeof(secret_code)) idx = 0; // loop around
    }
    
    uint8_t current = bit & 0b11000000;
    bit <<= 2;
    
    switch (current) {
    case 0b00000000: {
      // dit
      Entity::global_mask |= 0b00000001;
      duration = MORSE_TIME_UNIT;
      return NOTE_A4;
    } break;
    case 0b01000000: {
      // dah
      Entity::global_mask |= 0b00000001;
      duration = MORSE_TIME_UNIT * 3;
      return NOTE_A4;
    } break;
    case 0b10000000: {
      // character gap
      duration = MORSE_TIME_UNIT * 2;
    } break;
    default: {
      // word gap
      duration = MORSE_TIME_UNIT * 6;
    } break;
    }
    return 0;
}

void Sound::play(Type type) {
    if (Sound::type >= type) {
        // higher priority (lower type)
        // => override current sound effect
        Sound::type = type;
        Sound::idx = 0;
        Sound::duration = 1;
        bit = pgm_read_byte_near(&secret_code[0]) << 8;
        arg = 1;
    }
}

Sound::Type Sound::getType() {
    return (Type) Sound::type;
}

#else

void Sound::init() {
}

void Sound::update() {
}

void Sound::play(Type type) {
}

Sound::Type Sound::getType() {
    return Type::SILENT;  
}

#endif
