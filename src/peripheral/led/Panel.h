#pragma once

#include <Platform.h>

#include "Palette.h"

#include "../../util/NBitArray.h"

class Panel {

private:

  /**
   * storage for the level led colors
   */
  static NBitArray<LED_MAX_COUNT, COLOR_PALETTE_INDEX_BITS> indices;

  /**
   * timer used for periodic color changes
   */
  static int8_t timer;

  /**
   * computes the color index corresponding to the current timer
   *
   * @param c0
   * @param c1
   * @return
   *   periodic color idx
   */
  static uint8_t getPeriodic(uint8_t c0, uint8_t c1);

public:
  /**
   * Sets the LED PINs of the level and both life LEDs to output mode
   * and clears the colors
   */
  static void init();

  /**
   * clears all colors to 0,0,0
   */
  static void clear();

  /**
   * sets the color of the LED at position pos
   * to the color saved in palette[c]
   *
   * @param pos
   * @param c
   *    0 <= c < 32
   */
  static void draw(int16_t pos, uint8_t c);
  /**
   * sets the color of the LED at position pos
   * to a color saved between palette[c0] and palette[c1].
   * the exact index used depends on the timing.
   * basically, it will start at c0, go up to c1 over time
   * and go back to c0 over time again, repeat.
   *
   * switching c0 and c1 will shift the period by half a period.
   *
   * @param pos
   *    0 <= pos < LEVEL_LED_COUNT
   * @param c0, c1
   *    0 <= c0, c1 < 32
   */
  static void drawPeriodic(int16_t pos, uint8_t c0, uint8_t c1);
  /**
   * sets the color of the LEDs from pos to pos+dim
   * to the color saved in palette[c]
   *
   * @param pos
   * @param dim
   * @param c
   *    0 <= c < 32
   */
  static void drawLine(int16_t pos, int16_t dim, uint8_t c, bool force = false);

  /**
   * sets the color of the LEDs from pos to pos+dim
   * to a color saved between palette[c0] and palette[c1].
   * the exact index used depends on the timing.
   * basically, it will start at c0, go up to c1 over time
   * and go back to c0 over time again, repeat.
   *
   * switching c0 and c1 will shift the period by half a period.
   *
   * @param pos
   *    0 <= pos < LEVEL_LED_COUNT
   * @param c0, c1
   *    0 <= c0, c1 < 32
   */
  static void drawLinePeriodic(int16_t pos, int16_t dim, uint8_t c0, uint8_t c1);
  /**
   * sets the color of the LEDs from pos to pos+dim
   * by repeating the pattern of c0,c1,c2,c3
   * with an offset (offset of 1 => first color is c3, then c0,c1,c2,c3,...)
   *
   * @param pos
   * @param dim
   * @param offset
   * @param c0, c1, c2, c3
   *    0 <= c0, c1, c2, c3 < 32
   */
  static void drawLinePattern(int16_t pos, int16_t dim, uint16_t offset, const uint8_t* pattern, uint16_t pattern_length, bool force = false);

  /**
   * writes the appropriate color values for the given number of
   * lives to the live LED strips
   *
   * @param lives
   */
  static void drawLives(uint8_t lives);

  /**
   * writes the currently assigned LED colors onto the LED strip
   */
  static void show();

  /**
   * @return
   *    the current value of the periodic timer (also used for item animations)
   */
  static uint8_t getTimer();

};
