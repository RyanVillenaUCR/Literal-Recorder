/*
 * playback_task.h
 *
 * Created: 6/10/2018 2:31:09 PM
 *  Author: getaf
 */ 

#ifndef PLAYBACK_TASK_H
#define PLAYBACK_TASK_H

#include "LiteralRecorder1.h"
#include "note.h"
#include "pwm.h"

extern unsigned char mode;

enum STATES_PLAYBACK { PLAYBACK_START,
    PLAYBACK_OFF, PLAYBACK_PLAY};
 
extern unsigned char recordedSong[64];   

unsigned char tick_Playback(unsigned char state) {
    
    static unsigned char i;
    
    //Transitions
    switch (state) {
        
        case PLAYBACK_START:
            i = 0;
            state = PLAYBACK_OFF;
            break;
        
        case PLAYBACK_OFF:
            //state = (mode == FREE_PLAY ? PLAYBACK_PLAY : PLAYBACK_OFF);
            if (mode == FREE_PLAY) {
                
                state = PLAYBACK_PLAY;
                i = 0;
                
                eeprom_read_block((void*) recordedSong, (const void*) 12, 64);
                
            } else {
                
                state = PLAYBACK_OFF;
            }
            
            break;
            
        case PLAYBACK_PLAY:
            //state = (i < 64 ? PLAYBACK_PLAY : PLAYBACK_OFF);
            if (i < 64) {
                state = PLAYBACK_PLAY;
            } else {
                
                state = PLAYBACK_OFF;
                task_in_progress = false;
                set_PWM(0);
            }
            break;
            
        default:
            break;
    }
    
    //Actions
    switch (state) {
        
        case PLAYBACK_PLAY:
            set_PWM(frequencies[recordedSong[i]]);
            printNote(i, 1);
            LCD_DisplayDigits(false, i);
            i++;
            break;
            
        default:
            break;
    }
    return state;
}

#endif // PLAYBACK_TASK_H