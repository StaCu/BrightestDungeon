#pragma once

#include <stdint.h>

class WS2811 {

public:
  
  struct __attribute__ ((packed)) Color {
    // WS2811 uses GRB color coding
    uint8_t g;
    uint8_t r;
    uint8_t b;
  
    Color()
    : g(0), r(0), b(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b)
    : g(g), r(r), b(b) {}
  };

  void begin();
  void reset();
  void show(const Color *colors, const uint8_t *indices, uint8_t mode, uint16_t n);

private:
  /**
   * @param colors
   *   the colors
   * @param indices
   *   the indices,
   *   can be NULL, if the idx_bits is 0 or greater than 8
   * @param idx_bits
   *   0..8: indices are used to access the palette
   *         => in the special case of mode == 0
   *            indices can be null
   *   > 8 : indices are ignored
   *         => indices can be null
   *         => colors are transmitted to the leds directly
   *            and in order
   */
  void assembler(const Color *colors, const uint8_t *indices, uint8_t idx_bits, uint16_t n);
  void __attribute__((noinline)) assembler0(const Color *colors, const uint8_t *indices, uint8_t idx_bits, uint16_t n);
  
};
