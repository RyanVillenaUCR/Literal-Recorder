//This entire header file written by Ryan Villena.

#ifndef MYBIT_H
#define MYBIT_H

#include <stdbool.h>

unsigned char MySetBit(unsigned char bitstring, unsigned char index, unsigned char set) {

    unsigned char output = bitstring;

    if (set) {

        output = bitstring | (0x01 << index);

        } else {

        output = bitstring & ~(0x01 << index);
    }

    return output;
}

bool MyGetBit(unsigned char bitstring, unsigned char index) {

    //Shift index all the way to the right
    bitstring = bitstring >> index;

    //Compare to 0x01
    return (0x01 & bitstring) == 0x01;  //clear all other bits before comparing to 0000 0001
}

#endif // MYBIT_H
