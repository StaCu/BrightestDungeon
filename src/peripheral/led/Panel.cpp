#include "Panel.h"

#include "../../level/entity/EntityUnion.h"
#include "../../level/Options.h"
#include "../../util/Math.h"

NBitArray<LED_MAX_COUNT, COLOR_PALETTE_INDEX_BITS> Panel::indices;
int8_t Panel::timer;

void Panel::init() {
  Palette::updateBrightness();
  leds_init();
  leds_reset();
}

void Panel::clear() {
  indices.clear();
}

uint8_t Panel::getPeriodic(uint8_t c0, uint8_t c1) {
    if (Panel::timer >= 0) {
        return c0;
    } else {
        return c1;
    }
}

void Panel::draw(int16_t pos, uint8_t c) {
  Panel::drawLinePattern(pos, 1, 0, &c, 1);
}

void Panel::drawPeriodic(int16_t pos, uint8_t c0, uint8_t c1) {
  Panel::drawLinePeriodic(pos, 1, c0, c1);
}

void Panel::drawLine(int16_t pos, int16_t dim, uint8_t c, bool force) {
  Panel::drawLinePattern(pos, dim, 0, &c, 1, force);
}

void Panel::drawLinePeriodic(int16_t pos, int16_t dim, uint8_t c0, uint8_t c1) {
  uint8_t c = getPeriodic(c0, c1);
  drawLine(pos, dim, c);
}

void Panel::drawLinePattern(int16_t pos, int16_t dim, uint16_t offset, const uint8_t* pattern, uint16_t pattern_length, bool force) {
  if (pos < 0) {
    dim    += pos;
    offset += pos;
    pos = 0;
  } else
  if (pos + dim > Options::level_length) {
    dim = Options::level_length - pos;
  }
  if (dim <= 0) return;

  indices.set(pos+MAX_LIVES+LED_STATUS_COUNT+1, dim, offset, pattern, pattern_length, force);
}

void Panel::drawLives(uint8_t lives) {
  uint8_t color = COLOR_MONSTER_0;
  if (Hero::isAlive()) {
    color = Hero::getColorIdx();
  }
  if (LED_STATUS_COUNT > 0) {
    indices.set(0, LED_STATUS_COUNT, color);
  }
  indices.set(LED_STATUS_COUNT, lives, Hero::getColorIdx());
  indices.set(lives+LED_STATUS_COUNT, MAX_LIVES - lives, COLOR_MONSTER_0);
  indices.set(MAX_LIVES+LED_STATUS_COUNT, COLOR_WALL_0);
}

void Panel::show() {
    timer++;
    leds_show(indices.getDataPtr(), Palette::palette, COLOR_PALETTE_INDEX_BITS, Options::led_count);
}

uint8_t Panel::getTimer() {
    return timer;
}
