// ---------------------------------------------------------------------------
// Created by Tim Eckel - teckel@leethost.com
// Copyright 2016 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// See "toneAC.h" for purpose, syntax, version history, links, and more.
// ---------------------------------------------------------------------------

/**
 * NOTE REGARDING CHANGES TO ToneAC 
 * 
 * The code has been modified (mostly shortened) in order to tailor it specifically
 * for use with BrightestDungeon.
 */

#include "toneAC.h"
#include "../../Settings.h"

#ifndef SIMULATOR

#if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#define PWMT1A_MASK DDB1
#define PWMT1A_DREG DDRB
#define PWMT1A_PORT PORTB

#define PWMT1B_MASK DDB2
#define PWMT1B_DREG DDRB
#define PWMT1B_PORT PORTB

//uint8_t _tAC_volume[] = { 200, 100, 67, 50, 40, 33, 29, 22, 11, 2 }; // Duty for linear volume control.
uint8_t _tAC_volume[] = { 200, 100, 40, 2 }; // Duty for volume control.

void toneAC(uint16_t frequency, uint8_t volume) {
    if (frequency == 0 || volume == 0) {
        TIMSK1 &= ~_BV(OCIE1A);        // Remove the timer interrupt.
        TCCR1B  = _BV(CS11);           // Default clock prescaler of 8.
        TCCR1A  = _BV(WGM10);          // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
        PWMT1A_PORT &= ~_BV(PWMT1A_MASK); // Set timer 1 PWM pins to LOW.
        PWMT1B_PORT &= ~_BV(PWMT1B_MASK); // Other timer 1 PWM pin also to LOW.
        return;
    }
    
    PWMT1A_DREG |= _BV(PWMT1A_MASK);
    PWMT1B_DREG |= _BV(PWMT1B_MASK); // Set timer 1 PWM pins to OUTPUT (because analogWrite does it too).

    uint8_t prescaler = _BV(CS10);                  // Try using prescaler 1 first.
    unsigned long top = F_CPU / frequency / 2 - 1;  // Calculate the top.
    if (top > 65535) {                              // If not in the range for prescaler 1, use prescaler 256 (122 Hz and lower @ 16 MHz).
        prescaler = _BV(CS12);                        // Set the 256 prescaler bit.
        top = top / 256 - 1;                          // Calculate the top using prescaler 256.
    }

    ICR1   = top;                                     // Set the top.
    if (TCNT1 > top) TCNT1 = top;                     // Counter over the top, put within range.
    TCCR1B = _BV(WGM13)  | prescaler;                 // Set PWM, phase and frequency corrected (top=ICR1) and prescaler.
    OCR1A  = OCR1B = top / _tAC_volume[volume - 1];   // Calculate & set the duty cycle (volume).
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1B0); // Inverted/non-inverted mode (AC).
}

#else

int freq = 0;
int vol = 0;

void toneAC(uint16_t frequency, uint8_t volume) {
#if SHOW_SOUND
    if (volume == 255) {
        frequency = freq;
        volume = vol;
    } else {
        freq = frequency;
        volume = volume;
    }

    std::string text = "tone(f: " + std::to_string((int) frequency) + ",v: " + std::to_string((int) volume) + ")";
    //mvprintw(1, 0, text.c_str());
    std::cout << text << std::endl;
#endif
}

#endif
