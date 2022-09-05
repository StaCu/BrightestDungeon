#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#include <stdint.h>

#define AVR_PORTB 0x25
#define AVR_PORTC 0x28
#define AVR_PORTD 0x2b

class Pin {

public:

  template<uint8_t PORT, uint8_t PIN>
  static void setModeDigitalRead();
  
  template<uint8_t PORT, uint8_t PIN>
  static void setModeDigitalWrite();
  
  template<uint8_t PORT, uint8_t PIN>
  static bool read();
  
  template<uint8_t PORT, uint8_t PIN>
  static void write(const bool &value);
  
  template<uint8_t PORT, uint8_t PIN>
  static void writeHigh();
  
  template<uint8_t PORT, uint8_t PIN>
  static void writeLow();
  
  static uint16_t __attribute__((noinline)) readAnalog(uint8_t pin);
  
};

template<uint8_t PORT, uint8_t PIN>
void Pin::setModeDigitalRead() {
  // Set the Data Direction of the pin to 0
  // => the DataDirectionRegister is located at PORT-Address - 1
  asm volatile (
    "cbi %[DDR], %[PIN] \n"
    :
    : [DDR] "I" (PORT - __SFR_OFFSET - 1), [PIN] "I" (PIN)
    :
  );
}

template<uint8_t PORT, uint8_t PIN>
void Pin::setModeDigitalWrite() {
  // Set the Data Direction of the pin to 1
  // => the DataDirectionRegister is located at PORT-Address - 1
  asm volatile (
    "sbi %[DDR], %[PIN] \n"
    :
    : [DDR] "I" (PORT - __SFR_OFFSET - 1), [PIN] "I" (PIN)
    :
  );
}

template<uint8_t PORT, uint8_t PIN>
bool Pin::read() {
  // read the Pin value of the pin
  // => the PinRegister is located at PORT-Address - 2
  bool value;
  asm volatile (
    "clr  %[VALUE]        \n"
    "sbic %[PORT], %[PIN] \n"
    "ldi  %[VALUE], 1     \n"
    : [VALUE] "=d" (value)
    : [PORT] "I" (PORT - __SFR_OFFSET - 2), [PIN] "I" (PIN)
    :
  );
  return value;
}

template<uint8_t PORT, uint8_t PIN>
void Pin::write(const bool &value) {
  // write the Port value of the pin
  // => the PortRegister is located at PORT-Address
  volatile uint8_t output = value; //LOW or HIGH
  asm volatile (
    "sbrc %[VALUE], 0    \n"
    "sbi %[PORT], %[PIN] \n"
    "sbrs %[VALUE], 0    \n"
    "cbi %[PORT], %[PIN] \n"
    : 
    : [PORT] "I" (PORT - __SFR_OFFSET), [PIN] "I" (PIN),
      [VALUE] "r" (output)
    :
  );
}

template<uint8_t PORT, uint8_t PIN>
void Pin::writeHigh() {
  // write the Port value of the pin
  // => the PortRegister is located at PORT-Address
  asm volatile (
    "sbi %[PORT], %[PIN] \n"
    : 
    : [PORT] "I" (PORT - __SFR_OFFSET), [PIN] "I" (PIN)
    :
  );
}

template<uint8_t PORT, uint8_t PIN>
void Pin::writeLow() {
  // write the Port value of the pin
  // => the PortRegister is located at PORT-Address
  asm volatile (
    "cbi %[PORT], %[PIN] \n"
    : 
    : [PORT] "I" (PORT - __SFR_OFFSET), [PIN] "I" (PIN)
    :
  );
}
