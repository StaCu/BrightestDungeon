#pragma once

#include <stdint.h>

/**
 * This Timer does not really count time.
 * Instead it generates a 100ms signal with a static update
 * method, which is then used to update all timer objects
 * every 100ms.
 *
 * By using 100ms as interval, the range of the internal timer value
 * and interval can be reduced to only 8bit (16bit total).
 * Thus giving a range from 100ms (interval == 0) to 25600ms (interval == 255).
 */
class __attribute__ ((packed)) Timer {

private:
  static uint8_t global_decrement;
  static uint8_t global_timing;

  uint8_t timer;
  uint8_t interval;

public:
  /**
   * creates a new Timer
   * the timer will reset every 100ms
   * the first signal will come after 100ms
   */
  Timer();
  /**
   * creates a new Timer
   * the timer will reset every "(interval+1)*100ms".
   * the first signal will come after 100ms
   */
  Timer(const uint8_t interval);
  /**
   * creates a new Timer
   * the timer will reset every "(interval+1)*100ms".
   * the first signal is delayed by "(first+1)*100ms"
   */
  Timer(const uint8_t first, const uint8_t interval);

  Timer& operator=(const Timer& other);

  /**
   * decrements the internal timer, if the global decrement is
   * set (once every 100ms) and returns true, if the decrement
   * caused a reset.
   *
   * if this function is called in every update cycle, but not
   * those, where global_decrement is set, the timer will never
   * reset.
   */
  bool dec();

  /**
   * resets the timer to the interval value
   * this will cause a signal the next time, dec() is called
   */
  void resetLow();

  /**
   * resets the timer to the interval value
   */
  void resetHigh();

  /**
   * @return
   *      whether the next call to dec() that actually decreases the timer will return true
   */
  bool isOver() const;

  /**
   * adds to the current interval.
   * the change will be applied after the next signal
   *
   * Note:
   *  => be mindful of the 0..255 range of the timer
   *     variable, as this function has no saturating
   *     logic. it will overflow
   */
  void addInterval(int8_t i);

public:
  /**
   * updates the global timer signal
   * => call only once per update cycle
   * => signals whenever the "global_timing" value
   *    exceeds 100
   *    => if the update_time is no divisor of 100,
   *       the timing will not be off, when taking
   *       the average over multiple signals
   */
  static void updateTimer();

  /**
   * resets the global timer
   */
  static void resetTimer();

};

