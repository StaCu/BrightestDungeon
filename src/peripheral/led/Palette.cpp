#include "Palette.h"

#include "../../Settings.h"
#include "../../level/Options.h"

/**
 * the color palette
 * since the WS2811 requires the colors in GRB order,
 * this palette also uses GRB order
 *
 * Due to issues with the compiler, the values are
 * raw instead of WS2811::Color structs
 */
#if COLOR_PALETTE == 0

const uint8_t PROGMEM Palette::unscaled_palette[] = {
    0,   0,   0, // OFF, WATER_3

   64,  64, 128, // WATER_0
   32,  32,  64, // WATER_1
   16,  16,  32, // WATER_2
   16,  16,  16, // WATER_2

   16,  32,   0, // LAVA_OFF, MINE_0, ITEM_INVERT_1
  196, 255,  64, // LAVA_ON, MINE_1, ITEM_INVERT_0

   92, 128,  32, // PORTAL_ORANGE_0
   16,  32,   0, // PORTAL_ORANGE_1

   92,  32, 128, // PORTAL_BLUE_0
   16,   0,  32, // PORTAL_BLUE_1

    0,  64,  32, // SWITCH_OFF
    0, 255,  64, // SWITCH_ON
  224, 224, 224, // DOOR

  255,   0, 255, // ITEM
   32,   0,  32, // ITEM_ATTACK
   32,   0,   0, // ITEM_COLOR_0
    0,   0,  32, // ITEM_COLOR_1

   32,  32,  32, // WALL 0
  128, 128, 128, // WALL 1

   64, 255, 0  , // MONSTER_1
    0, 255, 0  , // MONSTER_0

   64, 255,  64, // FOG

  255, 0  , 255, // ATTACK
  255, 0  , 0  , // HERO_0
   32, 0  , 255, // HERO_1
};

#elif COLOR_PALETTE == 1

const uint8_t PROGMEM Palette::unscaled_palette[] = {
    0,   0,   0, // OFF, WATER_3

   64,  64, 128, // WATER_0
   32,   0, 128, // WATER_1
    0,   0, 128, // WATER_2
   32,  32,  32, // WATER_4

   16,  64,   0, // LAVA_OFF, ITEM_INVERT_1, COLOR_MINE_END
  196, 255,   0, // LAVA_ON, ITEM_INVERT_0, COLOR_MINE_BEGIN

   64, 255,   0, // PORTAL_ORANGE_0
   64, 128,  16, // PORTAL_ORANGE_1

   64,   0, 255, // PORTAL_BLUE_0
   64,  16, 128, // PORTAL_BLUE_1

    0,  64,  32, // SWITCH_OFF
    0, 255,  64, // SWITCH_ON
  224, 224, 224, // DOOR

  255,   0, 255, // ITEM
   32,   0,  32, // ITEM_ATTACK
   32,   0,   0, // ITEM_COLOR_0
    0,   0,  32, // ITEM_COLOR_1

   32,  32,  32, // WALL 0
  128, 128, 128, // WALL 1

   64, 255,   0, // MONSTER_1
    0, 255,   0, // MONSTER_0

   64, 255,  64, // FOG

  255,   0, 255, // ATTACK
  255,   0,   0, // HERO_0
    0,   0, 255, // HERO_1
};

#endif

WS2811::Color Palette::palette[sizeof(Palette::unscaled_palette)/3];

void Palette::updateBrightness() {
    const uint8_t* src = (uint8_t*) &unscaled_palette[0];
    uint8_t* dst = (uint8_t*) &palette[0];
    uint16_t scale[] = {LED_COLOR_CORRECT_G+1, LED_COLOR_CORRECT_R+1, LED_COLOR_CORRECT_B+1};
    for (uint8_t i = 0, channel = 0; i < sizeof(Palette::unscaled_palette); i++, channel++) {
        if (channel == 3) channel = 0;
        uint8_t unscaled = pgm_read_byte_near(&src[i]);
        dst[i] = (unscaled * scale[channel]) >> (8 + (5 - Options::brightness));
        if (unscaled != 0 && dst[i] == 0) dst[i] = 1;
    }
}
