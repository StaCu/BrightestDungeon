#pragma once

#include <stdint.h>

/**
 * Memory efficient bit storage.
 * This array can store and update data types smaller than 8bit 
 * It acts as a Depth-Buffer (from 3D Graphics)
 * This means that values will only increase with successive writes to the same entry.
 * 
 * @tparam n 
 *    number of entiries
 * @tparam bits
 *    number of bits per entry
 */
template<uint16_t n, uint8_t bits>
class NBitArray {

private:
  uint8_t data[((n*bits)/8)+1];

public:

  /**
   * sets all values to 0
   */
  void clear();

  /**
   * sets the color of the LED at position pos
   * to the color saved in palette[c]
   * 
   * only increases the current value
   *
   * @param idx
   *    the index
   * @param value
   *    0 <= value < 32
   */
  void set(uint16_t idx, uint8_t value);

  /**
   * sets the value of the slots from pos to pos+dim
   * to the value saved in palette[c]
   *
   * only increases the current values
   * 
   * @param idx
   *    the start index
   * @param length
   *    the length
   * @param value
   *    0 <= value < 32
   */
  void set(uint16_t idx, uint16_t length, uint8_t value);

  /**
   * sets the value of the slots from pos to pos+dim
   * by repeating the pattern of c0,c1,c2,c3
   * with an offset (offset of 1 => first color is c3, then c0,c1,c2,c3,...)
   *
   * only increases the current values (except when force is true)
   * 
   * @param idx
   *    the start index
   * @param length
   *    the length
   * @param offset
   *    the pattern offset
   * @param pattern
   *    0 <= c0, c1, c2, c3 < 32
   * @param pattern_length
   *    the pattern length
   * @param force
   *    update, even if the new value is smaller
   */
  void set(uint16_t idx, uint16_t length, uint16_t offset, const uint8_t* pattern, uint16_t pattern_length, bool force = false);

  uint8_t* getDataPtr();

  const inline uint16_t byteLength() const;

  uint8_t get(uint16_t idx);

};

template<uint16_t n, uint8_t bits>
void NBitArray<n,bits>::clear() {
  for (uint16_t i = 0; i < byteLength(); i++) {
    data[i] = 0;
  }
}

template<uint16_t n, uint8_t bits>
void NBitArray<n,bits>::set(uint16_t idx, uint8_t value) {
  set(idx, 1, 0, &value, 1);
}

template<uint16_t n, uint8_t bits>
void NBitArray<n,bits>::set(uint16_t idx, uint16_t length, uint8_t value) {
  set(idx, length, 0, &value, 1);
}

template<uint16_t n, uint8_t bits>
void NBitArray<n,bits>::set(uint16_t idx, uint16_t length, uint16_t offset, const uint8_t* pattern, uint16_t pattern_length, bool force) {
  uint16_t bit_pos  = idx*bits;
  uint16_t idx0     = bit_pos / 8;

  // the shift is at most 8 bits
  // => the last 3 bit of the bit position
  //    are the shift
  uint8_t shift = bit_pos - (idx0 * 8);

  uint16_t value = pattern_length -1 - offset;
  while (length > 0) {
    value++;
    if (value >= pattern_length) {
      value = 0;
    }

    uint8_t v = pattern[value];
    v &= (0xFF >> (8 - bits));
    uint16_t &slot = *((uint16_t*) &data[idx0]);
    uint8_t old_value = (slot >> shift) & (0xFF >> (8 - bits));
    if (v > old_value || force) {
      // only update if the new value is larger
      // => sort of like a z-buffer in computer graphics
      uint16_t mask_zero =  (((uint16_t) v) << shift);
      uint16_t mask_one  = ~(((uint16_t)(0xFF >> (8 - bits))) << shift);
      slot &= mask_one;
      slot |= mask_zero;
    }
    
    shift += bits;
    if (shift >= 8) {
      idx0++;  
      shift -= 8;
    }    
    length--;  
  }
}

template<uint16_t n, uint8_t bits>
uint8_t* NBitArray<n,bits>::getDataPtr() {
  return data;
}

template<uint16_t n, uint8_t bits>
const inline uint16_t NBitArray<n,bits>::byteLength() const {
  return ((n*bits)/8)+1;
}

template<uint16_t n, uint8_t bits>
uint8_t NBitArray<n,bits>::get(uint16_t idx) {
  uint16_t bit_pos  = idx*bits;
  uint16_t idx0     = bit_pos / 8;

  // the shift is at most 8 bits
  // => the last 3 bit of the bit position
  //    are the shift
  uint8_t shift = bit_pos - (idx0 * 8);

  uint16_t slot = *((uint16_t*) &data[idx0]);
  uint8_t value = (slot >> shift) & (0xFF >> (8 - bits));
  return value;
}
