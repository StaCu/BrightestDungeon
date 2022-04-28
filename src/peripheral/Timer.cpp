#include "Timer.h"

#include "../Settings.h"

uint8_t Timer::global_decrement = 0;
uint8_t Timer::global_timing = 0;

Timer::Timer()
 : Timer(0, 0) {

}
Timer::Timer(const uint8_t interval)
 : Timer(1, interval) {

}
Timer::Timer(const uint8_t first, const uint8_t interval)
 : timer(first+1), interval(interval) {

}

Timer& Timer::operator=(const Timer& other) {
  timer = other.timer;
  interval = other.interval;
  return *this;
}

bool Timer::dec() {
  if (global_decrement == 1) {
    if (timer == 0) {
      // the timer does nothing in this case
    } else if (timer == 1) {
      timer = interval;
      return true;
    } else {
      timer--;
    }
  }
  return false;
}

void Timer::resetLow() {
  timer = 1;
}

void Timer::resetHigh() {
  timer = interval;
}

bool Timer::isOver() const {
  return timer == 1;
}

void Timer::addInterval(int8_t i) {
  interval += i;
}

void Timer::updateTimer() {
  // use a 16 bit variable temporarily for the computation
  // to avoid overflow errors
  uint16_t tmp = global_timing;
  if (tmp + UPDATE_TIME >= 100) {
    // 250ms passed, but since it could have been
    // more than 250ms, we need to keep track of the
    // error
    // => subtract 250 instead of starting from 0
    global_timing = tmp + UPDATE_TIME - 100;
    // signal that 250ms have passed
    global_decrement = 1;
  } else {
    global_timing = tmp + UPDATE_TIME;
    global_decrement = 0;
  }
}

void Timer::resetTimer() {
  global_timing = 0;
}
