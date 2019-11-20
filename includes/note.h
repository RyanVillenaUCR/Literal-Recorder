//This entire header file written by Ryan Villena.

#ifndef NOTE_H
#define NOTE_H


#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//#include <assert.h>
//#include <stdbool.h>
#include <string.h>
//#include "LiteralRecorder1.h"
#include "joystick_io.h"
#include "lcd_io.h"
//#include "bit.h"
//#include "pwm.h"
//#include "adc_init.h"
#include "mybit.h"
#include "mic.h"

//NOT IN ORDER OF 4-BIT MAPPING
const char* allNoteNames[] = {  //simply for traversal of whole steps/half steps
    "A2", //accessible with joystick jumper AND heavy vibrato               // 0
    "A#2 / Bb2", "B2",  //these are only accessible using joystick jumper   // 1  2
    "C3", "C#3 / Db3", "D3",                                                // 3  4  5
    "D#3 / Eb3", "E3", "F3",                                                // 6  7  8
    "F#3 / Gb3", "G3", "G#3 / Ab3",                                         // 9 10 11
    "A3", "A#3 / Bb3", "B3",                                                //12 13 14
    "C4", "C#4 / Db4", "D4",                                                //15 16 17
    "D#4 / Eb4", "E4", "F4",                                                //18 19 20
    "F#4 / Gb4", "G4", "G#4 / Ab4",                                         //21 22 23
    "A4", "A#4 / Bb4", "B4",                                                //24 25 26
    "C5", "C#5 / Db5", "D5",                                                //27 28 29
    "D#5 / Eb5", "E5",   //these are only accessible using joystick jumper  //30 31
    "F5", //accessible with joystick jumper AND heavy vibrato               //32
    "shh" //mute                                                            //33
};

const unsigned char LENGTH_ALLNOTENAMES = 34;

const char* rawNoteNames[] = { //In order of 4-bit mapping, e.g. C3 is 0000, D5 is 1111
    "C3", "D3", "E3", "F3",
    "G3", "A3", "B3", "C4",
    "D4", "E4", "F4", "G4",
    "A4", "B4", "C5", "D5"
};

//Elements here correspond to those in whiteNoteNames
//These elements are their note's corresponding index in allNoteNames
const char rawIndicesWithinAll[] = {
    3,  5,  7,  8,
    10, 12, 14, 15,
    17, 19, 20, 22,
    24, 26, 27, 29
};

const double frequencies[] = {
    110.000,
    116.541, 123.471,
    130.813, 138.591, 146.832,
    155.563, 164.814, 174.614,
    184.997, 195.998, 207.652,
    220.000, 233.082, 246.942,
    261.626, 277.183, 293.665,
    311.127, 329.628, 349.228,
    369.994, 391.995, 415.305,
    440.000, 466.164, 493.883,
    523.251, 554.365, 587.330,
    622.254, 659.255,
    698.456,
    0   //mute
};

const unsigned char FLAT[]  = { 0x08, 0x08, 0x08, 0x0E, 0x0A, 0x0C, 0x08 };
const unsigned char SHARP[] = { 0x0A, 0x0B, 0x1E, 0x0A, 0x0F, 0x1A, 0x0A };

const unsigned char FLAT_ADDR  = 0;
const unsigned char SHARP_ADDR = 1;

//const unsigned char JOY_LR_PIN = 3;
//const unsigned char JOY_UD_PIN = 0;
const unsigned char SHH_INDEX = 33;
//const unsigned char BUTTON_PIN = 2;

void buildSymbols() {
    
    LCDBuildChar(SHARP_ADDR, SHARP);
    LCDBuildChar(FLAT_ADDR, FLAT);
}

unsigned char safeInc_note(unsigned char allIndex) {
    
    //Has to be less than length-1, at most length-2
    //length-1 is reserved for vibrato ONLY
    if (allIndex + 1 <= LENGTH_ALLNOTENAMES - 2) {
        allIndex++;
    }
    
    return allIndex;
}

unsigned char safeDec_note(unsigned char allIndex) {
    
    //Has to be greater than 0, at least 1
    //0 is reserved for vibrato ONLY
    if (allIndex - 1 >= 1) {
        allIndex--;
    }
    
    return allIndex;
}

//+-2 for HI inputs, +-1 for LO inputs
unsigned char modifyIndexWith_joyInput(unsigned char i_currentNote, unsigned char joyInput) {
    
    switch (joyInput) {
        
        case UP_HI:
        i_currentNote = safeInc_note(i_currentNote);
        case UP_LO:
        i_currentNote = safeInc_note(i_currentNote);
        break;
        
        case DOWN_HI:
        i_currentNote = safeDec_note(i_currentNote);
        case DOWN_LO:
        i_currentNote = safeDec_note(i_currentNote);
        break;
        
        case NO_JOY:
        break;
    }
    
    return i_currentNote;
}

////Returns index to the note in allNoteNames
//unsigned char getNote() {
    //
    ////If they're not blowing, don't generate a sound
    ////if (!getMicBinary(1)) return SHH_INDEX;   //comment out to disable mic checking
    ////if (!MyGetBit(~PINA, BUTTON_PIN)) return SHH_INDEX;    //use button connected to PA2 instead
    //
    ////_delay_ms(20);    //Taken care of by ADC_in task
    //
    //
    ////Gather button input
    //unsigned char i_currentNote = ~PIND & 0x0F;   //only take lower 4 bits
    //i_currentNote = rawIndicesWithinAll[i_currentNote];
    ////now, i_currentNote is an index of the All array
//
    ////Gather accidental input
    //unsigned char joyInput = getJoystickDiscrete(JOY_UD_PIN);
    //i_currentNote = modifyIndexWith_joyInput(i_currentNote, joyInput);
    //
    ////_delay_ms(20);    //Taken care of by ADC_in task
    //
    //return i_currentNote;
//}

//Returns index to the note in allNoteNames
unsigned char getNote(bool mic_arg, unsigned char ud_arg) {
    
    //If they're not blowing, don't generate a sound
    //if (!getMicBinary(1)) return SHH_INDEX;   //comment out to disable mic checking
    //if (!MyGetBit(~PINA, BUTTON_PIN)) return SHH_INDEX;    //use button connected to PA2 instead
    if (!mic_arg) return SHH_INDEX;
    
    //_delay_ms(20);    //Taken care of by ADC_in task
    
    
    //Gather button input
    unsigned char i_currentNote = ~PIND & 0x0F;   //only take lower 4 bits
    i_currentNote = rawIndicesWithinAll[i_currentNote];
    //now, i_currentNote is an index of the All array

    //Gather accidental input
    //unsigned char joyInput = getJoystickDiscrete(JOY_UD_PIN);
    unsigned char joyInput = ud_arg;
    i_currentNote = modifyIndexWith_joyInput(i_currentNote, joyInput);
    
    //_delay_ms(20);    //Taken care of by ADC_in task
    
    return i_currentNote;
}

//Ranges in either direction from freq of referenceNote
//Max value of one half-step above, min value of one half-step below
//referenceNoteIndex MUST NOT be 0 or frequencies.length-1
//Can be positive or negative
//double getVibrato(unsigned char referenceNoteIndex) {
double getVibrato(unsigned char referenceNoteIndex, short lr_value) {    

    if (referenceNoteIndex == SHH_INDEX) return 0;
    
    //Take input
    //short input = getJoystickValue(JOY_LR_PIN);
    short input = lr_value;
    input -= 512;   //"center" value around 0 instead of 512
    
    //_delay_ms(20);    //Taken care of by ADC_in task
    
    //Calculations:
    //Positive value
    //input / 512.0 = vibrato / (hiFreq - homeFreq)
    //vibrato = (input / 512.0) * (hiFreq - homeFreq)
    //
    //Negative value
    //input / 512.0 = vibrato / (homeFreq - loFreq)
    //vibrato = (input / 512.0) * (homeFreq - loFreq)
    //In addition, divide by rangeReducer to get less wonky output
    //for example, rangeReducer = 2 will halve the output frequency range
    
    double homeFreq = frequencies[referenceNoteIndex];
    const double RANGE_REDUCER = 4.0;

    if (input > 0) {
        
        double hiFreq = frequencies[referenceNoteIndex + 1];
        return ((double) input / 512.0) * (hiFreq - homeFreq) / RANGE_REDUCER;
        
        } else if (input < 0) {
        
        double loFreq = frequencies[referenceNoteIndex - 1];
        return ((double) input / 512.0) * (homeFreq - loFreq) / RANGE_REDUCER;
        
    } else {
        
        return 0;
    }
    
}

void produceSound(unsigned char indexOfNote, double vibrato) {
    
    if (indexOfNote == SHH_INDEX) {
        set_PWM(0);
    } else {
        set_PWM(frequencies[indexOfNote] + vibrato);
    }
}

//@TODO: fix bug - indexOfNote anything other than 1 puts incorrect output
void printNote(unsigned char indexOfNote, unsigned char column) {
    
    ////Don't print if you're already printing the same thing
    //static unsigned char lastIndex;
    //if (indexOfNote == lastIndex) {
        //
        //return;
    //}    
    //lastIndex = indexOfNote;
    
    //discards const qualifier
    char* noteStr = (char* ) allNoteNames[indexOfNote];
    
    //Black key - necessitates special characters
    if (strlen(noteStr) == 9) {
        
        LCD_ClearLine(true);
        //LCD_ClearScreen();
        
        //First note name
        //LCD_WriteSubstring(1, noteStr, 1);
        LCD_WriteSubstring(column + 0, noteStr, 1);
        
        //First note sharp
        //LCD_Cursor(2);
        LCD_Cursor(column + 1);
        LCD_WriteData(SHARP_ADDR);
        
        //all in between sharp & flat
        //LCD_WriteSubstring(3, noteStr + 2, 5);
        LCD_WriteSubstring(column + 2, noteStr + 2, 5);
        
        //Second note flat
        //LCD_Cursor(8);
        LCD_Cursor(column + 7);
        LCD_WriteData(FLAT_ADDR);
        
        //Second note number
        //LCD_WriteSubstring(9, noteStr + 8, 1);
        LCD_WriteSubstring(column + 8, noteStr + 8, 1);
        
    }
    
    //White key - just print like usual
    else {
        
        LCD_DisplayLine(true, noteStr);
        //LCD_DisplayString(column, noteStr);
    }
}


#endif // NOTE_H