#include "../../Settings.h"

#ifdef USE_SIMPLE_CONTROLLER

#include "Controller.h"
#include "../Pin.h"
#include "../../level/Options.h"

int8_t angle_buffer[2];

void Controller::init() {
  // Set attack pin to input mode and high
  Pin::setModeDigitalRead<ATTACK_PIN>();
  Pin::writeHigh<ATTACK_PIN>();
}

void Controller::callibrate() {
  int16_t ax = Pin::readAnalog(SIMPLE_CONTROLLER_PIN_MOVE) >> 2;
  if (ax > Options::joystick_max) {
    Options::joystick_max = ax;
  } else if (ax < Options::joystick_min) {
    Options::joystick_min = ax;
  }
  Options::joystick_mid = ax;
}

void Controller::poll(int8_t &angle, bool &attack) {
  /**
   * Move:
   *   read the analog value of the move pin
   */
  int16_t ax = Pin::readAnalog(SIMPLE_CONTROLLER_PIN_MOVE) >> 2;

  if (ax < Options::joystick_min) {
    ax = Options::joystick_min;
  } else if (ax > Options::joystick_max) {
    ax = Options::joystick_max;
  }
  ax -= Options::joystick_mid;
  if (ax < -SIMPLE_CONTROLLER_DEADZONE) {
    ax += SIMPLE_CONTROLLER_DEADZONE;
    ax = (ax * 100) / (Options::joystick_mid - Options::joystick_min);
  } else if (ax > SIMPLE_CONTROLLER_DEADZONE) {
    ax -= SIMPLE_CONTROLLER_DEADZONE;
    ax = (ax * 100) / (Options::joystick_max - Options::joystick_mid);
  } else {
    ax = 0;
  }

/*
 * the potentiometer I used was fairly consistent, i.e. when not moving,
 * the reported value remained almost constant.
 * in such a case, the following code can be left out.
 * It smoothes the value by applying a 3 wide median filter.
 *
 * on the other hand, the code requires almost no memory at all, so one
 * can just include it.
 */
  // find median angle
  int8_t median_angle = (int8_t) ax;
  if (angle_buffer[0] < median_angle) {
    if (angle_buffer[1] < median_angle) {
      median_angle = angle_buffer[0] > angle_buffer[1] ? angle_buffer[0] : angle_buffer[1];
    }
  } else if (angle_buffer[1] > median_angle) {
    median_angle = angle_buffer[0] < angle_buffer[1] ? angle_buffer[0] : angle_buffer[1];
  }

  // add new sample to angle_buffer
  angle_buffer[1] = angle_buffer[0];
  angle_buffer[0] = ax;

  angle = median_angle;


  angle = ax;

  /**
   * Attack:
   *   read the digital value of the attack pin
   */
  attack = !Pin::read<ATTACK_PIN>();
}

#endif
