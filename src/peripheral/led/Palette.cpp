#include "Palette.h"

#include "../../level/Options.h"

/**
 * the color palette
 */
#ifndef USE_HIGHER_CONTRAST_COLOR_PALETTE

const uint8_t PROGMEM Palette::unscaled_palette[] = {
    COLOR_SWIZZLE(  0,   0,   0), // OFF, WATER_3

    COLOR_SWIZZLE( 32,  16,   0), // LAVA_OFF, MINE_0, ITEM_INVERT_1

    COLOR_SWIZZLE( 64,  64, 128), // WATER_0
    COLOR_SWIZZLE( 32,  32,  64), // WATER_1
    COLOR_SWIZZLE( 16,  16,  32), // WATER_2
    COLOR_SWIZZLE( 16,  16,  16), // WATER_2

    COLOR_SWIZZLE(128,  92,  32), // PORTAL_ORANGE_0
    COLOR_SWIZZLE( 32,  16,   0), // PORTAL_ORANGE_1

    COLOR_SWIZZLE( 32,  92, 128), // PORTAL_BLUE_0
    COLOR_SWIZZLE(  0,  16,  32), // PORTAL_BLUE_1

    COLOR_SWIZZLE( 64,   0,  32), // SWITCH_OFF
    COLOR_SWIZZLE(255,   0,  64), // SWITCH_ON
    COLOR_SWIZZLE(224, 224, 224), // DOOR

    COLOR_SWIZZLE(  0, 255, 255), // ITEM
    COLOR_SWIZZLE(  0,  32,  32), // ITEM_ATTACK
    COLOR_SWIZZLE(  0,  32,   0), // ITEM_COLOR_0
    COLOR_SWIZZLE(  0,   0,  32), // ITEM_COLOR_1

    COLOR_SWIZZLE(255, 196,  64), // LAVA_ON, MINE_1, ITEM_INVERT_0

    COLOR_SWIZZLE( 32,  32,  32), // WALL 0
    COLOR_SWIZZLE(128, 128, 128), // WALL 1

    COLOR_SWIZZLE(255,  64, 0  ), // MONSTER_1
    COLOR_SWIZZLE(255,   0, 0  ), // MONSTER_0

    COLOR_SWIZZLE(255,  64,  64), // FOG

    COLOR_SWIZZLE(0  , 255, 255), // ATTACK
    COLOR_SWIZZLE(0  , 255, 0  ), // HERO_0
    COLOR_SWIZZLE(0  ,  32, 255), // HERO_1
};

#else

const uint8_t PROGMEM Palette::unscaled_palette[] = {
    COLOR_SWIZZLE(  0,   0,   0), // OFF, WATER_3

    COLOR_SWIZZLE( 64,  16,   0), // LAVA_OFF, ITEM_INVERT_1, COLOR_MINE_END

    COLOR_SWIZZLE( 64,  64, 128), // WATER_0
    COLOR_SWIZZLE(  0,  32, 128), // WATER_1
    COLOR_SWIZZLE(  0,   0, 128), // WATER_2
    COLOR_SWIZZLE( 32,  32,  32), // WATER_4

    COLOR_SWIZZLE(255,  64,   0), // PORTAL_ORANGE_0
    COLOR_SWIZZLE(128,  64,  16), // PORTAL_ORANGE_1

    COLOR_SWIZZLE(  0,  64, 255), // PORTAL_BLUE_0
    COLOR_SWIZZLE( 16,  64, 128), // PORTAL_BLUE_1

    COLOR_SWIZZLE( 64,   0,  32), // SWITCH_OFF
    COLOR_SWIZZLE(255,   0,  64), // SWITCH_ON
    COLOR_SWIZZLE(224, 224, 224), // DOOR

    COLOR_SWIZZLE(  0, 255, 255), // ITEM
    COLOR_SWIZZLE(  0,  32,  32), // ITEM_ATTACK
    COLOR_SWIZZLE(  0,  32,   0), // ITEM_COLOR_0
    COLOR_SWIZZLE(  0,   0,  32), // ITEM_COLOR_1

    COLOR_SWIZZLE(255, 196,   0), // LAVA_ON, ITEM_INVERT_0, COLOR_MINE_BEGIN

    COLOR_SWIZZLE( 32,  32,  32), // WALL 0
    COLOR_SWIZZLE(128, 128, 128), // WALL 1

    COLOR_SWIZZLE(255,  64,   0), // MONSTER_1
    COLOR_SWIZZLE(255,   0,   0), // MONSTER_0

    COLOR_SWIZZLE(255,  64,  64), // FOG

    COLOR_SWIZZLE(  0, 255, 255), // ATTACK
    COLOR_SWIZZLE(  0, 255,   0), // HERO_0
    COLOR_SWIZZLE(  0,   0, 255), // HERO_1
};

#endif

uint8_t Palette::palette[sizeof(Palette::unscaled_palette)];

void Palette::updateBrightness() {
    const uint8_t* src = (uint8_t*) &unscaled_palette[0];
    uint8_t* dst = (uint8_t*) &palette[0];
    uint16_t scale[] = {COLOR_SWIZZLE(LED_COLOR_CORRECT_R+1, LED_COLOR_CORRECT_G+1, LED_COLOR_CORRECT_B+1)};
    for (uint8_t i = 0, channel = 0; i < sizeof(Palette::unscaled_palette); i++, channel++) {
        if (channel == 3) channel = 0;
        uint8_t unscaled = platform_read_byte_progmem(&src[i]);
        dst[i] = (unscaled * scale[channel]) >> (8 + (5 - Options::brightness));
        if (unscaled != 0 && dst[i] == 0) dst[i] = 1;
    }
}
