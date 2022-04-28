// ---------------------------------------------------------------------------
// toneAC Library - v1.4 - 10/02/2017
//
// AUTHOR/LICENSE:
// Created by Tim Eckel - teckel@leethost.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// LINKS:
// Project home: https://bitbucket.org/teckel12/arduino-toneac/wiki/Home
// Blog: http://forum.arduino.cc/index.php?topic=142097.0
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
// Replacement to the standard tone library with the advantage of nearly twice
// the volume, higher frequencies (even if running at a lower clock speed),
// higher quality (less clicking), nearly 1.5k smaller compiled code and less
// stress on the speaker. Disadvantages are that it must use certain pins and
// it uses two pins instead of one. But, if you're flexible with your pin
// choices, this is a great upgrade. It also uses timer 1 instead of timer 2,
// which may free up a conflict you have with the tone library. It exclusively 
// uses port registers for the fastest and smallest code possible.
//
// USAGE:
// Connection is very similar to a piezo or standard speaker. Except, instead
// of connecting one speaker wire to ground you connect both speaker wires to
// Arduino pins. The pins you connect to are specific, as toneAC lets the
// ATmega microcontroller do all the pin timing and switching. This is
// important due to the high switching speed possible with toneAC and to make
// sure the pins are alyways perfectly out of phase with each other
// (push/pull). See the below CONNECTION section for which pins to use for
// different Arduinos. Just as usual when connecting a speaker, make sure you
// add an inline 100 ohm resistor between one of the pins and the speaker wire.
//
// CONNECTION:
//   Pins  9 & 10 - ATmega328, ATmega128, ATmega640, ATmega8, Uno, Leonardo, etc.
//   Pins 11 & 12 - ATmega2560/2561, ATmega1280/1281, Mega
//   Pins 12 & 13 - ATmega1284P, ATmega644
//   Pins 14 & 15 - Teensy 2.0
//   Pins 25 & 26 - Teensy++ 2.0
//
// SYNTAX:
//   toneAC( frequency [, volume [, length [, background ]]] ) - Play a note.
//     Parameters:
//       * frequency  - Play the specified frequency indefinitely, turn off with noToneAC().
//       * volume     - [optional] Set a volume level. (default: 10, range: 0 to 10 [0 = off])
//       * length     - [optional] Set the length to play in milliseconds. (default: 0 [forever], range: 0 to 4294967295 [49.7 days])
//       * background - [optional] Play note in background or pause till finished? (default: false, values: true/false)
//   noToneAC()  - Stop playing.
//
// HISTORY:
//
// 10/02/2017 V1.4 - Added NOTONEAC and PLAY_FOREVER macros for code clarity
// and improved readability. ~Gabriel Staples, www.ElectricRCAircraftGuy.com
//
// 08/19/2016 v1.3 - Fixed to work with Teensy++ 2.0 and probably the 1.0
// model as well. Cleaned up and organized code which resulted in smaller
// compiled code size and no longer the need for the TONEAC_TINY switch and
// alternate method.
//
// 01/27/2013 v1.2 - Fixed a counter error which went "over the top" and caused
// periods of silence (thanks Krodal). For advanced users needing tight code,
// the TONEAC_TINY switch in toneAC.h activates a version of toneAC() that
// saves 30-130 bytes depending on controller. With TONEAC_TINY, the syntax is
// toneAC_tiny(frequency, length) which plays the note at full volume in the
// background. Added support for the ATmega 640, 644, 1281, 1284P and 2561
// microcontrollers.
//
// 01/16/2013 v1.1 - Option to play notes in background, returning control back
// to your sketch for processing while note plays (similar to the way the tone
// library works). Volume is now linear and in the range from 0-10. Now uses
// prescaler 256 instead of 64 for frequencies below 122 Hz so it can go down
// to 1 Hz no matter what speed the CPU is clocked at (helpful if using toneAC
// to control a two-pin dual LED).
//
// 01/11/2013 v1.0 - Initial release.
//
// ---------------------------------------------------------------------------


/**
 * NOTE REGARDING CHANGES TO ToneAC 
 * 
 * The code has been modified (mostly shortened) in order to tailor it specifically
 * for use with BrightestDungeon.
 */
#pragma once

#include <stdint.h>

/**
 * configure the PWM hardware to generate a continous output with a given
 * frequency and volume
 * 
 * if (frequency == 0 || volume == 0)
 *      this will turn the PWM off
 * 
 * @param frequency 
 *      the frequency
 * @param volume 
 *      the volume [0..4]
 */
void toneAC(uint16_t frequency, uint8_t volume);
