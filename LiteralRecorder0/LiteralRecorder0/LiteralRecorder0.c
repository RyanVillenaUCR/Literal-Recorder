/*
 * LiteralRecorder0.c
 *
 * Created: 5/30/2018 5:35:12 AM
 *  Author: getaf
 */ 

#ifndef LITERAL_RECORDER_0
#define LITERAL_RECORDER_0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <assert.h>
#include <stdbool.h>
//#include <eeprom.h>
#include <avr/eeprom.h>
#include "joystick_io.h"
#include "lcd_io.h"
#include "timer.h"
#include "bit.h"
#include "pwm.h"
#include "adc_init.h"
#include "mybit.h"
#include "mic.h"
#include "scheduler.h"
#include "note.h"

unsigned char EEMEM song_inEEPROM[64];

//const unsigned char MIC_PIN = 1;
//const unsigned char JOY_UD_PIN = 3;
//const unsigned char JOY_LR_PIN = 0;
//const unsigned char BACKUP_BUTTON_PIN = 2;

//# warning "child love"

//const unsigned char SHH_INDEX = 33;
//const unsigned char LENGTH_ALLNOTENAMES = 34;

////NOT IN ORDER OF 4-BIT MAPPING
//const char* allNoteNames[] = {  //simply for traversal of whole steps/half steps
    //"A2", //accessible with joystick jumper AND heavy vibrato               // 0
    //"A#2 / Bb2", "B2",  //these are only accessible using joystick jumper   // 1  2
    //"C3", "C#3 / Db3", "D3",                                                // 3  4  5
    //"D#3 / Eb3", "E3", "F3",                                                // 6  7  8
    //"F#3 / Gb3", "G3", "G#3 / Ab3",                                         // 9 10 11
    //"A3", "A#3 / Bb3", "B3",                                                //12 13 14
    //"C4", "C#4 / Db4", "D4",                                                //15 16 17
    //"D#4 / Eb4", "E4", "F4",                                                //18 19 20
    //"F#4 / Gb4", "G4", "G#4 / Ab4",                                         //21 22 23
    //"A4", "A#4 / Bb4", "B4",                                                //24 25 26
    //"C5", "C#5 / Db5", "D5",                                                //27 28 29
    //"D#5 / Eb5", "E5",   //these are only accessible using joystick jumper  //30 31
    //"F5", //accessible with joystick jumper AND heavy vibrato               //32
    //"shh" //mute                                                            //33
//};
//
//const char* rawNoteNames[] = { //In order of 4-bit mapping, e.g. C3 is 0000, D5 is 1111
    //"C3", "D3", "E3", "F3",
    //"G3", "A3", "B3", "C4",
    //"D4", "E4", "F4", "G4",
    //"A4", "B4", "C5", "D5"
//};
//
////Elements here correspond to those in whiteNoteNames
////These elements are their note's corresponding index in allNoteNames
//const char rawIndicesWithinAll[] = {
     //3,  5,  7,  8,
    //10, 12, 14, 15,
    //17, 19, 20, 22,
    //24, 26, 27, 29
//};
//
//const double frequencies[] = {
    //110.000,
    //116.541, 123.471,
    //130.813, 138.591, 146.832,
    //155.563, 164.814, 174.614,
    //184.997, 195.998, 207.652,
    //220.000, 233.082, 246.942,
    //261.626, 277.183, 293.665,
    //311.127, 329.628, 349.228,
    //369.994, 391.995, 415.305,
    //440.000, 466.164, 493.883,
    //523.251, 554.365, 587.330,
    //622.254, 659.255,
    //698.456,
    //0   //mute
//};
//
//Utility function taken from Harsh Agarwal
//www.geeksforgeeks.org/program-binary-decimal-conversion/
unsigned char bitsToChar(unsigned char n) {
    
    unsigned char num = n;
    unsigned char dec_value = 0;
    
    unsigned char base = 1;
    unsigned char temp = num;
    
    while (temp) {
        
        unsigned char last_digit = temp % 10;
        temp = temp/10;
        
        dec_value += last_digit*base;
        
        base *= 2;
    }
    
    return dec_value;
}

void initialize() {
    
    //Joystick input (only need PA0)
    DDRA = 0x00;
    PORTA = 0xFF;
        
    //LCD display output
    DDRC = 0xFF;
    PORTC = 0x00;
        
    //PD0-PD3: Button input
    //PD6-PD7: LCD output, for some reason
    DDRD = 0xC0;    //1100 0000
    PORTD = 0x3F;   //0011 1111
    
    //PB6: Speaker output
    DDRB = 0xFF;
    PORTB = 0x00;
        
    //Enable ADC to be able to read analog values from PA0
    ADC_init();
        
    //Prep LCD
    LCD_init();
    
    //Enable PWM
    PWM_on();
        
    //Initialize timer
    TimerSet(100);
    TimerOn();

    buildSymbols();
}

void displayJoyInput() {
    
    unsigned char joyInput = getJoystickDiscrete(JOY_UD_PIN);
    switch (joyInput) {
        
        case NO_JOY:
        LCD_DisplayString(17, "Center");
        break;
        
        case UP_LO:
        LCD_DisplayString(17, "up lo");
        break;
        
        case UP_HI:
        LCD_DisplayString(17, "UP HI");
        break;
        
        case DOWN_LO:
        LCD_DisplayString(17, "down lo");
        break;
        
        case DOWN_HI:
        LCD_DisplayString(17, "DOWN HI");
        break;
        
        default:
        LCD_DisplayString(17, "InvalidInput");
        break;
    }
}

int main(void) {
    
    initialize();

    //LCD_DisplayString(1, "LiteralRecorder0");
    
    unsigned char i = 0;
    bool doneRecording = false;
    
    unsigned char measures = 8;
    unsigned char song_inRAM[8 * measures]; //smallest increment is 8th note
    
    LCD_displayTwoLines("Left: Record", "Right: Playback");
    while (MyGetBit(PINA, 4) && MyGetBit(PINA, 2)) {}
        
    //left - record
    if (!MyGetBit(PINA, 4)) {
        
        LCD_DisplayString(1, "Recording!");
        while (!MyGetBit(PINA, 4)) {}
            
        while(MyGetBit(PINA, 4) && !doneRecording) {
                
            //Calculate note
            unsigned char noteIndex = getNote();
            _delay_ms(20);  //need to do this between polls
            double vibratoFreq = getVibrato(noteIndex);
                
            //Record note
            if (i % 3 == 0) {   //Every 300ms
                    
                song_inRAM[i] = noteIndex;
            }
            if (i >= 64)
            doneRecording = true;
            i++;
                
            //Display current sound on LCD
            printNote(noteIndex, 1);
                
            //Make sound
            produceSound(noteIndex, vibratoFreq);
            //produceSound(noteIndex, 0.0);
                
            //Do a timer delay
            while (!TimerFlag) {}
            TimerFlag = 0;
        }
            
        produceSound(SHH_INDEX, 0);
            
        //Write to EEPROM
        eeprom_update_block( (const void*) song_inRAM, (void*) song_inEEPROM, 8 * measures);
        //LCD_DisplayString(1, "Wrote to EEPROM");
    }
    
    //right - playback
    else if (!MyGetBit(PINA, 2)) {
        
        while (!MyGetBit(PINA, 2));
        //by now, they've lifted finger off 2
        
        eeprom_read_block((void*) song_inRAM, (const void*) song_inEEPROM, 8 * measures);
            
        //Just playback the song
        for (unsigned char* note_ptr = song_inRAM;
        note_ptr <= song_inRAM + (8 * measures);
        note_ptr++) {
                
            produceSound(*note_ptr, 0);
                
            while (!TimerFlag) {
                produceSound(*note_ptr, 0);
            }
            TimerFlag = false;
        }
    }
    
    //LCD_DisplayString(1, "Done! RESET for another go :)");
    LCD_displayTwoLines("Done! RESET for", "another go :)");

    
    
    
    return 0;
}

#endif // LITERAL_RECORDER_0
