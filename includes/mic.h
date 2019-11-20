//This header file and all functions herein written by Ryan Villena.

#ifndef MIC_H
#define MIC_H

#include <avr/io.h>
#include <stdbool.h>

unsigned short THRESHOLD = 160;

unsigned short getMic(unsigned char pin) {

    ADMUX = pin;
    return ADC;
}

bool getMicBinary(unsigned char pin) {

    return getMic(pin) >= THRESHOLD;
}

#endif // MIC_H
