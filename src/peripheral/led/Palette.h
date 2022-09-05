#pragma once

#include <Platform.h>

enum Color : uint8_t {
  COLOR_OFF,

  COLOR_WATER_0,
  COLOR_WATER_1,
  COLOR_WATER_2,
  COLOR_WATER_4,

  COLOR_LAVA_OFF,
  COLOR_LAVA_ON,

  COLOR_PORTAL_ORANGE_BEGIN,
  COLOR_PORTAL_ORANGE_END,

  COLOR_PORTAL_BLUE_BEGIN,
  COLOR_PORTAL_BLUE_END,

  COLOR_SWITCH_OFF,
  COLOR_SWITCH_ON,
  COLOR_DOOR,

  COLOR_ITEM,
  COLOR_ITEM_ATTACK,
  COLOR_ITEM_COLOR_0,
  COLOR_ITEM_COLOR_1,

  COLOR_WALL_0,
  COLOR_WALL_1,

  COLOR_MONSTER_1,
  COLOR_MONSTER_0,

  COLOR_FOG,

  COLOR_ATTACK,
  COLOR_HERO_0,
  COLOR_HERO_1,
  // ----

  COLOR_PALETTE_COUNT,

  COLOR_MINE_BEGIN = COLOR_LAVA_ON,
  COLOR_MINE_END = COLOR_LAVA_OFF,

  COLOR_WATER_3 = COLOR_OFF,

  COLOR_ITEM_INVERT_CONTROL_0 = COLOR_LAVA_ON,
  COLOR_ITEM_INVERT_CONTROL_1 = COLOR_LAVA_OFF,
};

class Palette {

public:

  /**
   * color palette
   */
  static const uint8_t PROGMEM unscaled_palette[];
  /**
   * color palette
   */
  static uint8_t palette[];

  static void updateBrightness();

};
