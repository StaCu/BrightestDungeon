#include "Panel.h"

#include "../../level/entity/EntityUnion.h"
#include "../../level/Options.h"

NBitArray<LED_MAX_COUNT, 5> Panel::indices;
int8_t Panel::timer;

WS2811 Panel::leds;

void Panel::init() {
  Palette::updateBrightness();
  leds.begin();
}

void Panel::clear() {
  indices.clear();
}

uint8_t Panel::getPeriodic(uint8_t c0, uint8_t c1) {
  uint8_t abs = Panel::timer > 0 ? Panel::timer : -Panel::timer;
  if (c1 < c0) {
    uint8_t tmp = c1;
    c1 = c0;
    c0 = c1;
  }

  uint8_t dif = c1 - c0;
  uint8_t idx = ((abs * dif + 64) >> 7);

  idx += c0;
  return idx;
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
    leds.show((WS2811::Color*) &Palette::palette, indices.getDataPtr(), 5, Options::led_count);

#ifdef SIMULATOR
    uint8_t colors[]     = {  0, 37, 94, 36, 36, 33, 33, 33, 33, 34, 94, 37, 37, 37, 36, 36, 32, 34, 37, 90, 31, 31, 91, 36, 32, 34};
    uint8_t characters[] = {' ','X','=','-','-','=','L','-','=','-','=','s','S','D','#','-','-','-','W','w','M','M','F','A','H','H'};

    std::string leds = "[";
    for (uint64_t i = 0; i < Options::led_count; i++) {
        uint8_t val = indices.get(i);
        leds += "\033[1;";
        leds += std::to_string(colors[val]);
        leds += "m";
        leds += (char) characters[val];
        leds += "\033[0m";
    }
    leds += "]";
   // mvprintw(0, 0, leds.c_str());
    std::cout << leds << std::endl;
#endif
}

uint8_t Panel::getTimer() {
    return timer;
}
