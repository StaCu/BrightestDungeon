#include "Pin.h"

#define PS2   (1<<ADPS0)                             //8000kHz ADC clock freq
#define PS4   (1<<ADPS1)                             //4000kHz
#define PS8   ((1<<ADPS0) | (1<<ADPS1))              //2000kHz
#define PS16  (1<<ADPS2)                             //1000kHz
#define PS32  ((1<<ADPS2) | (1<<ADPS0))              //500kHz
#define PS64  ((1<<ADPS2) | (1<<ADPS1))              //250kHz
#define PS128 ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)) //125kHz
#define ANALOG_V_REF     DEFAULT //INTERNAL, EXTERNAL, or DEFAULT
#define ADC_PRESCALE     PS128   //PS16, PS32, PS64 or P128(default)

uint16_t Pin::readAnalog(uint8_t pin) {
  uint16_t analog = 0;
  #ifdef SIMULATOR
    if (Simulator::getButton() == 'd') {
      analog = 0;
    } else if (Simulator::getButton() == 'a') {
      analog = 1023;
    } else {
      analog = 511;
    }
  #else
  asm volatile (
  "andi %1, 0x07    \n" //force pin==0 thru 7
  "ori  %1, (%6<<6) \n" //(pin | ADC Vref)
  "sts  %2, %1      \n" //set ADMUX

  "lds  r18, %3             \n" //get ADCSRA
  "andi r18, 0xf8           \n" //clear prescale bits
  "ori  r18, ((1<<%5) | %7) \n" //(new prescale | ADSC)
  "sts  %3, r18             \n" //set ADCSRA

  "_loop:       \n" //loop until ADSC cleared
  "lds  r18, %3 \n"
  "sbrc r18, %5 \n"
  "rjmp _loop   \n"

  "lds  %A0, %4   \n" //result = ADCL
  "lds  %B0, %4+1 \n" //ADCH

  : "=r" (analog) : "r" (pin), "M" (_SFR_MEM_ADDR(ADMUX)),
  "M" (_SFR_MEM_ADDR(ADCSRA)), "M" (_SFR_MEM_ADDR(ADCL)),
  "I" (ADSC), "I" (ANALOG_V_REF), "M" (ADC_PRESCALE)
  : "r18"
  );
  #endif
  return analog;
}
