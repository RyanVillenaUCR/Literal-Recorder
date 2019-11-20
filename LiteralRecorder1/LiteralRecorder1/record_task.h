/*
 * record_task.h
 *
 * Created: 6/10/2018 3:09:55 PM
 *  Author: getaf
 */ 

#ifndef RECORD_TASK_H
#define RECORD_TASK_H

#include "LiteralRecorder1.h"
#include "note.h"
#include <avr/eeprom.h>

extern unsigned char mode;
extern unsigned char recordedSong[64];

extern unsigned char ud;
extern unsigned char lr;
extern bool mic;

enum STATES_RECORD { RECORD_START,
    RECORD_OFF, RECORD_RECORD, RECORD_DONE };
    
unsigned char tick_Record(unsigned char state) {
    
    static unsigned char i;
    
    //Transitions
    switch (state) {
        
        case RECORD_START:
        
            i = 0;
            state = RECORD_OFF;
            break;
            
        case RECORD_OFF:
        
            if (mode == RECORD) {
                
                state = RECORD_RECORD;
                i = 0;
                LCD_DisplayLine(false, "Mode: Record");
                    
            } else {
                state = RECORD_OFF;
            }
            break;
        
        case RECORD_RECORD:
        
            if (i < 64) {
                
                state = RECORD_RECORD;
                
                //If recording a new song, delete the old one
                
                unsigned char my_temp[64];
                memset((void*) my_temp, SHH_INDEX, 64);
                eeprom_update_block((const void*) my_temp, (void*) 12, 64);
            } else {
                
                state = RECORD_DONE;
                i = 0;
            }
        
            break;
            
        case RECORD_DONE:
        
            state = RECORD_OFF;
            break;
    }
    
    
    //Actions
    switch (state) {
        
        case RECORD_RECORD:
        
            ;   //c is weird
            unsigned char temp = getNote(mic, ud);
            printNote(temp, 1);
            //LCD_DisplayLine(false, i);
            LCD_DisplayDigits(false, i);
            //unsigned char temp = getNote(mic, ud);
            recordedSong[i] = temp;
            
            i++;
            break;
            
        case RECORD_DONE:
        
            eeprom_update_block((const void*) recordedSong, (void*) 12, 64);
            i = 0;
            task_in_progress = false;
            
        default:
            break;
    }
    
    return state;
}


#endif // RECORD_TASK_H
