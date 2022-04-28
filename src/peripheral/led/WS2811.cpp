#include "WS2811.h"

#include "../../Settings.h"

#ifdef SIMULATOR

void WS2811::begin() {}
void WS2811::reset() {}
void WS2811::show(const Color *colors, const uint8_t *indices, uint8_t mode, uint16_t n) {}
void WS2811::assembler(const Color *colors, const uint8_t *indices, uint8_t idx_bits, uint16_t n) {}
void __attribute__((noinline)) WS2811::assembler0(const Color *colors, const uint8_t *indices, uint8_t idx_bits, uint16_t n) {}

#else

extern "C" {
extern volatile unsigned long timer0_millis;
#    define MS_COUNTER timer0_millis
};

#define PIN_TO_PORT(pin) PORTB
#define PIN_TO_PIN(pin) PORTB5

#define LOW8(x) x & 0x00ff
#define HIGH8(x) x >> 8

void WS2811::begin() {
  // Set digital pin 5 to output mode and low
  asm volatile (
    "sbi %0, %1 \n"
    :
    : "I" (_SFR_IO_ADDR(DDRB)) , "I" (DDB0)
    :
  );
}

void WS2811::reset() {
  // set pin to low and wait 50 microseconds
  // 50 microseconds are 800 cycles
  
  asm volatile (
    // set pin to low
  "cbi %0, %1 \n"
    
    // each loop has 4 cycles
    // => repeat 200 times for
    //    800 cycles
    "ldi r16, 200        \n"
    
    "wait%=:             \n"
    "dec r16             \n"
    "nop                 \n"
    "brne wait%=         \n"   
    :
    : "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB0)
    : "r16"
  );
}

void WS2811::show(const Color *colors, const uint8_t *indices, uint8_t idx_bits, uint16_t n) {
  assembler(colors, indices, idx_bits, n);
}

void WS2811::assembler(const Color *colors, const uint8_t *indices, uint8_t idx_bits, uint16_t n) {
  assembler0(colors, indices, idx_bits, n);

  uint32_t cycles = 13 + ((uint32_t) n * 24 * 20) - 4;
  // 1 cycle == 62.5 ns
  // 1ms = 160000 cycles

  // => -1 ms, because one interrupt is stored
  uint32_t millisec = (cycles / 16000);

  if (millisec > 1) {
    millisec -= 1;
    MS_COUNTER += millisec;
  }
}

void WS2811::assembler0(const Color *colors, const uint8_t *indices, uint8_t idx_bits, uint16_t n) {
  
  /*
   * WS2811 Transmission protocol
   * 
   * The Arduino Uno clocks with 16Mhz
   * This gives a time per cycle of 1/16Mhz = 62.5ns
   * 
   * Sending data:
   * 
   * 0:     |<-T0H->|<--T0L-->|
   * 1:     |<--T1H-->|<-T1L->|
   * reset: |<- Tres ->|
   * 
   *                              cycles:
   * T0H  =   500ns +- 150ns   => 8    +- 2
   * T1H  =  1200ns +- 150ns   => 19.2
   * T0L  =  2000ns +- 150ns   => 32
   * T1L  =  1300ns +- 150ns   => 20.8
   * Tres = 50000ns ++         => 800
   * 
   * Half the time if using high speed mode
   * Tres is the same in high speed mode
   * 
   * this implementation uses the fast mode:
   *                              cycles:
   * T0H  =   250ns +-  75ns   => 4    +- 1
   * T1H  =   600ns +-  75ns   => 9.6
   * T0L  =  1000ns +-  75ns   => 16
   * T1L  =   650ns +-  75ns   => 10.4
   */

  asm volatile (
  /*
   * reset delay would only be necessary,
   * if the leds had been written less than 50ms before
   * => I assume that whatever the rest of the application
   *    does, it takes more than 50ms
   * => if not, call
   *      reset()
   *    between the two calls to write the same led strip
   */

  /*
   * Notes:
   * 1. setting the data pin to high/low is marked by
   *    <T0H/T1H>, <T0L>, <T1L> in the respective lines.
   *    They are placed according to the time values given
   *    in the comment showing the WS2811 protocol
   * 2. in front of every assembler instruction is a comment
   *    noting the current cycle for easier understanding of
   *    the timing.
   *    As there are 20 cycles available to send one bit, the
   *    numbers range from 0 to 19, "  " for instructions outside
   *    the critical section
   * 3. combining 1 and 2 results in <T0H/T1H> starting in cycle 0,
   *    <T0L> starting in cycle 4 and <T1L> starting in cycle 10.
   *    Deviating from those numbers was not necessary, even though
   *    the LEDs would allow for it to be of by 1 cycle.
   * 4. only the lvl0 of <T0L> needs some sort of condition. <T0H/T1H>
   *    always happens in cycle 0 and <T1L> always happens in cycle 10,
   *    even when transmitting a 0, because the pin would already be low
   *    in that case anyway.
   */

  // ################################
  // ##  INITIALIZE                ##
  // ################################

      /*
       * to compute the index for the color table, we need a bitmask
       * masking all relevant bits inside a byte.
       * if there are 5 index bits, then the mask must be 0b00011111
       *
       * the following loop creates this bitmask and saves it in
       * "static_idx_bit_mask" (r13)
       * in case "idx_bits" is larger than 8, the bitmask is set to 0,
       * because a value larger than 8 means that the index mode is not
       * used and chain mode is used instead.
       */

      /*  */"mov r10, %[IDX_BITS]      \n" // tmp = idx_bits; // use temporary register to hold idx_bits
      /*  */"clr r13                   \n" // static_idx_bit_mask = 0;
      /*  */"cpi %[IDX_BITS], 9        \n" // if ( idx_bits < 9 ) {
      /*  */"brsh init_idx_bit_mask_done%= \n"
      /*  */"init_idx_bit_mask%=:      \n" //   while ( tmp - 1 != 0 ) {
      /*  */"dec r10                   \n" //     tmp--;
      /*  */"brmi init_idx_bit_mask_done%= \n"
      /*  */"lsl r13                   \n" //     static_idx_bit_mask <<= 1;
      /*  */"inc r13                   \n" //     static_idx_bit_mask++;
      /*  */"rjmp init_idx_bit_mask%=  \n" //
      /*  */"init_idx_bit_mask_done%=: \n" // }

      /*
       * since 20 cycles aren't enough to do everything on the fly,
       * the transmission loop relies on pipelining.
       *   i.e: while the current LED is transmitted, the next one is loaded.
       * this means that the first LED must be loaded before entering the loop.
       * (it also means that while the last LED is transmitted, another LED is
       *  loaded, which - of course - contains garbage values.
       *  this is not an issue though, as the garbage values have no side effect)
       *
       * the following section loads the first LED
       * (X holds the address of the Color of the first LED)
       *
       * Note that this works for both, the index mode and the chain mode, because
       * in the case of the chain mode, the bitmask is 0 resulting in
       * color[0].
       */

      /*  */"movw r28, %A[INDICES]   \n" // // load address of indices into Y
      /*  */"ld r10, Y+              \n" // idx = *indices; indices++; // load first byte from indices
      /*  */"mov r12, r13            \n" // idx_bit_mask = static_idx_bit_mask;
      /*  */"and r12, r10            \n" // idx_bit_mask &= idx;
      /*  */"ldi r22, 3              \n" // shifts_to_do = 3;
      /*  */                             // // shifts_to_do is used as a temporary
      /*  */                             // // multiply by 3 because a color consists of 3 bytes
      /*  */"mul r12, r22            \n" // idx_bit_mask *= shifts_to_do;
      /*  */                             // // multiplication needs 2 cycles on atmega328p
      /*  */"movw r26, %A[COLORS]    \n" // X = colors;
      /*  */"add r26, r0             \n" //
      /*  */"adc r27, r1             \n" // X += idx_bit_mask;
      /*  */"mov r22, %[IDX_BITS]    \n" // shifts_to_do = idx_bits;
      /*  */"clr r23                 \n" // valid_idx_bits = 0;
      /*  */                             // // valid_idx_bits refers to "next_idx", used for pipelining.
      /*  */                             // // it has not been used yet, so it contains no valid bits

      /*
       * jumping to "load_channel" means the bits don't
       * need to be initialized, only the first led must
       * be loaded already (as done in the previous section).
       *
       * jumping to "load_channel" causes the first channel to be
       * loaded, without setting the data pin to high.
       * (this is only done to save a few bytes of program memory,
       *  no other reason)
       *
       * the time critical section begins here, therefore
       * interrupts must be disabled
       */

      /*  */"ldi r18, 0              \n" // channel_left = 0;
      /*  */"cli                     \n" // disable interrupts
      /*  */"rjmp load_channel%=     \n"

      // ################################
      // ##  SENDING ...               ##
      // ################################

      /*
       * As mentioned earlier, the loop uses pipelining, because
       * the complete index computation doesn't fit within one 20
       * cycle iteration.
       *
       * The solution may look somewhat like a finite state machine.
       *
       * Since there are 24 bits per LED, there are 24 iterations
       * per LED, 3 of which are used to move on to the next channel.
       * In other words, 21 iterations can be used to compute the
       * next LEDs colors address.
       * This is way more than enough time :)
       */

      /*
       * "load_led" is located in front of the main loop, to
       * save 1 cycle, as there is no need to jump to the lvl0
       * of the loop after "load_led" finished
       *
       * "load led" is divided into the following parts:
       *  - load next byte from index array if necessary
       *  - shift by "idx_bits" (one shift per iteration)
       *  - cut idx and add it to the base pointer of the
       *    color palette
       */

      //- - - - - - - - - - - - - - - - -//  load_led {
      /*  */"load_led%=:             \n" //
      /* 7*/"dec r21                 \n" //    load_led_calls_left--;
      /* 8*/"brne shift_idx%=        \n" //    if ( load_led_calls_left == 0 ) {
      /*  */                             //      // "load_led" has been called
      /*  */                             //      // 21 times (7 times per channel)
      /*  */                             //      // => shift is already complete
      /*  */                             //      // => compute the address
      /* 9*/"ldi r22, 3              \n" //      shifts_to_do = 3; // shifts_to_do used as a temporary
      /*10*/"cbi %[PORT], %[PIN]     \n" // <T1L>
      /*11*/"mul r12, r22            \n" //      idx_bit_mask *= shifts_to_do;
      /*12*/                             //
      /*13*/"movw r26, %A[COLORS]    \n" //      X = colors;
      /*14*/"add r26, r0             \n" //
      /*15*/"adc r27, r1             \n" //      X += idx_bit_mask;
      /*16*/"mov r22, %[IDX_BITS]    \n" //      shifts_to_do = idx_bits;
      /*17*/"nop                     \n" //
      /*18*/"rjmp loop_start%=       \n" //
      /*  */"prepare%=:              \n" //    } else if ( (shifts_to_do-1) < 0 ) {
      /*  */                             //      // careful: the jump towards this location
      /*  */                             //      // is located below (again, to save one cycle)
      /*  */                             //
      /*  */                             //      // do stuff that can be done before the
      /*  */                             //      // "load_led_calls_left == 0"-branch
      /*  */                             //      // (again, to save some cycles)
      /*14*/"mov r12, r13            \n" //      idx_bit_mask = static_idx_bit_mask;
      /*15*/"and r12, r10            \n" //      idx_bit_mask &= idx;
      /*17*/"inc r22                 \n" //      shifts_to_do++; // this is required to ensure
      /*18*/"rjmp loop_start%=       \n" //                      // this branch is taken
      /*  */                             //                      // again the next time
      /*  */                             //                      // (and not the "do_shift" branch)
      /*  */"shift_idx%=:            \n" //    } else {
      /*10*/"cbi %[PORT], %[PIN]     \n" // <T1L>
      /*11*/"dec r22                 \n" //      shifts_to_do--;
      /*12*/"brmi prepare%=          \n" //      if ( shifts_to_do >= 0 ) {
      /*13*/"dec r23                 \n" //        valid_idx_bits--;
      /*14*/"brpl do_shift%=         \n" //        if ( valid_idx_bits < 0 ) {
      /*  */                             //          // no more valid bits in the next_idx byte
      /*  */                             //          // shifting would result in the MSB of idx
      /*  */                             //          // being invalid
      /*  */                             //          // => load the next idx byte
      /*15*/"ld r11, Y+              \n" //          next_idx = *indices; indices++;
      /*16*/                             //
      /*17*/"ldi r23, 7              \n" //          valid_idx_bits = 7;
      /*  */                             //          // 7 instead of 8, because this branch triggers
      /*  */                             //          // on "< 0" not "== 0"
      /*  */"do_shift%=:             \n" //        }
      /*18*/"lsr r11                 \n" //        next_idx >>= 1; // do the actual shift
      /*19*/"ror r10                 \n" //        idx      >>= 1;
      /*  */                             //      } else {
      /*  */                             //        // go to the "(shifts_to_do-1) < 0"-branch
      /*  */                             //      }
      /*  */                             //    }
      //- - - - - - - - - - - - - - - - -//  };

      /*
       * the transmission loop is divided into the following parts:
       *  - lvl0 T0H,T1H,T0L,T1L according to the current bit
       *  - cycle bits
       *  - load the next channel every 8 bits
       *  - call "load_led" whenever possible
       */

      /*  */"loop_start%=:           \n" //  while ( true ) {
      /* 0*/"sbi %[PORT], %[PIN]     \n" // <T0H/T1H>
      /* 1*/"lsl r15                 \n" //    bits <<= 1;  // cycle to next bit
      /*  */                             //                 // the bit is moved into the carry flag
      /* 2*/"dec r17                 \n" //    bits_left--; // => one valid bit less remains
      /* 3*/"brcs skip_cbi_for_T0H%= \n" //
      /* 4*/"cbi %[PORT], %[PIN]     \n" // <T0L> // only call if carry flag is 0
      /*  */"skip_cbi_for_T0H%=:     \n" //
      /*  */                             //    if ( bits_left != 0 ) {
      /*  */                             //      // loading the next LED takes a long time
      /*  */                             //      // => call it in every cycle possible
      /* 5*/"brne load_led%=         \n" //      goto load_led;
      /*  */                             //    } else {
      /*  */                             //      // no more bits left
      /*  */"load_channel%=:         \n" //      // => load next channel
      /* 6*/"dec r18                 \n" //      channel_left--;
      /* 7*/"brpl load_channel_09%=  \n" //      if ( channel_left < 0 ) {
      /*  */                             //        // no next channel for the current led
      /*  */                             //        // => go on to the next one
      /* 8*/"ldi r18, 2              \n" //        channel_left = 2;
      /*  */                             //        // set to 2, because this branch triggers
      /*  */                             //        // on "channel_left < 0" (not == 0)
      /* 9*/"ldi r21, 21             \n" //        load_led_calls_left = 21;
      /*  */                             //        // in the following iterations, the next led
      /*  */                             //        // will be loaded, there are a total of 21
      /*  */                             //        // iterations, until it needs to be ready
      /*10*/"cbi %[PORT], %[PIN]     \n" // <T1L>
      /*11*/"subi %A[N], 1           \n" //        leds_left--;
      /*12*/"sbci %B[N], 0           \n" //        // if leds_left == 0 after the decrement, the next "load_led"
      /*  */                             //        // will load a garbage value, but this is not harmful,
      /*  */                             //        // because it has no side effects (read only).
      /*13*/"brpl load_channel_15%=  \n" //        if ( leds_left < 0 ) {
      /*  */                             //          // no more bits and channels and leds left
      /*  */                             //          // => exit loop
      /*  */"rjmp finish%=           \n" //          break;
      /*  */                             //        }
      /*  */"load_channel_09%=:      \n" //      } else {
      /* 9*/"cpi %[IDX_BITS], 9      \n" //        if ( IDX_BITS >= 9 ) {
      /*10*/"cbi %[PORT], %[PIN]     \n" // <T1L>    // no index table is used in this case
      /*11*/"brlo load_channel_13%=  \n" //          // => prevent computing the index based color
      /*12*/"inc r21                 \n" //          load_led_calls_left++;
      /*  */                             //          // incrementing this will cause the
      /*  */                             //          // "load_led_calls_left == 0"-branch of "load_led" to
      /*  */                             //          // never be called
      /*  */"load_channel_13%=:      \n" //        }
      /*13*/"nop                     \n" //
      /*14*/"nop                     \n" //
      /*  */"load_channel_15%=:      \n" //      }
      /*15*/"ldi r17, 8              \n" //      bits_left = 8; // new channel means 8 valid bits
      /*16*/"ld r15, X+              \n" //      bits = channel[X]; X++; // load the actual channel value
      /*17*/                             //                              // and move X to the next channel
      /*18*/"rjmp loop_start%=       \n" //    }
      //---------------------------------//  }

      // ################################
      // ##  ... SENDING               ##
      // ################################

      /*  */"finish%=:               \n"
      /*  */"sei                     \n" // reenable interrupts

  :
  : [PORT] "I" (_SFR_IO_ADDR(PORTB)), [PIN] "I" (PORTB0),
  [N] "d" (n),
  [COLORS] "r" (colors), [INDICES] "r" (indices),
  [IDX_BITS] "d" (idx_bits)
  :  "r0",  "r1",
     "r10", "r11", "r12", "r13", "r15",
     "r16", "r17", "r18", "r21", "r22", "r23",
     "r26", "r27", "r28", "r29" // X, Y
  );
}

// ################
// ##            ##
// ################

#endif