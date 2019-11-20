/*
 * freeplay_task.h
 *
 * Created: 6/10/2018 5:49:13 PM
 *  Author: getaf
 */ 

#ifndef FREEPLAY_TASK_H
#define FREEPLAY_TASK_H

#include "LiteralRecorder1.h"
#include "note.h"

extern unsigned char mode;

extern bool mic;
extern unsigned char ud;
extern unsigned char lr;

enum STATES_FREEPLAY {
    FREEPLAY_START,
    FREEPLAY_OFF, FREEPLAY_PLAYING
    };

unsigned char tick_Freeplay(unsigned char state) {
    
    static unsigned char lastNote;
    
    //Transitions
    switch (state) {
        
        case FREEPLAY_START:
            state = FREEPLAY_OFF;
            lastNote = SHH_INDEX;
            break;
            
        case FREEPLAY_OFF:
            //state = (mode == FREE_PLAY ? FREEPLAY_PLAYING : FREEPLAY_OFF);
            if (mode == FREE_PLAY) {
                
                printNote(SHH_INDEX, 1);
                LCD_DisplayLine(false, "Mode: Free Play");
                set_PWM(0);
                lastNote = SHH_INDEX;
                
                state = FREEPLAY_PLAYING;
                
            } else {
                
                state = FREEPLAY_OFF;
            }
            break;
            
        case FREEPLAY_PLAYING:
            //state = (getButtonInput(true) ? FREEPLAY_OFF : FREEPLAY_PLAYING);
            if (getButtonInput(true)) {
                
                state = FREEPLAY_OFF;
                task_in_progress = false;
                
            } else {
                state = FREEPLAY_PLAYING;
            }
            
            break;
    }
    
    //Actions
    switch (state) {
        
        case FREEPLAY_PLAYING:
        
            ;
            unsigned char temp = getNote(mic, ud);
            //if (temp == lastNote) {
                //break;
            //}
            
            set_PWM(frequencies[temp] + getVibrato(temp, lr_actual));
            
            if (temp != lastNote) {
                printNote(temp, 1);
            }
                        
            lastNote = temp;
            
            break;
            
        default:
            break;
            
    }
    return state;
}

#endif // FREEPLAY_TASK_H
