//This header file and all functions herein written by Ryan Villena.

#ifndef JOYSTICK_IO_H
#define JOYSTICK_IO_H

#include <assert.h>
#include <avr/io.h>


enum JOYSTICK_INPUT_ENUM {
    NO_JOY,
    UP_LO, DOWN_HI,
    UP_HI, DOWN_LO};    //too slo

const unsigned short NONE_RANGE = 50;
const unsigned short LO_RANGE = 300;

const unsigned short DOWN_BOUND = 0;
const unsigned short UP_BOUND = 1023;
const unsigned short MID = 512;

//Takes input from ADC
short getJoystickValue(unsigned char pin) {
    
    ADMUX = pin;
    return ADC;
}

unsigned char getJoystickDiscrete(unsigned char pin) {
    
    unsigned short temp = getJoystickValue(pin);

    if (MID-NONE_RANGE <= temp && temp <= MID+NONE_RANGE) {
        return NO_JOY;
        } else if (MID-LO_RANGE <= temp && temp < MID-NONE_RANGE) {
        return DOWN_LO;
        } else if (DOWN_BOUND <= temp && temp < MID-LO_RANGE) {
        return DOWN_HI;
        } else if (MID+NONE_RANGE < temp && temp <= MID+LO_RANGE) {
        return UP_LO;
        } else if (MID+LO_RANGE < temp && temp <= UP_BOUND) {
        return UP_HI;
        } else {
        //assert(0);
        return NO_JOY;
    }
}

char* msgFromJoyEnum(unsigned char joyInput) {
    
    switch (joyInput) {
        
        case NO_JOY:
        return "Center";
        
        case DOWN_LO:
        return "down_lo";
        
        case DOWN_HI:
        return "DOWN_HI";
        
        case UP_LO:
        return "up_lo";
        
        case UP_HI:
        return "UP_HI";
        
        default:
        return "invalid input";
    }
}

#endif // JOYSTICK_IO_H