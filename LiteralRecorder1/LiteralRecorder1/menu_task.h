
#ifndef MENU_TASK_H
#define MENU_TASK_H

#include "LiteralRecorder1.h"

extern unsigned char mode;


extern unsigned char lr;
extern unsigned char ud;

extern bool task_in_progress;

enum STATES_MENU {
    MENU_START,
    MENU_DISP,
    MENU_LEFT_WAIT, MENU_LEFT_ACTIVE,
    MENU_MODE_PRESS, MENU_MODE_RUNNING
};

//void disp_flashing(char* dispMe, unsigned char i) {
    //
    ////Assuming timer period of 10ms
    ////Flash on for 300ms, off for 300ms
    //
    ////const unsigned char TIMER_PERIOD = 100;
    //
    //if (i % 6 <= 3) {
        ////if (i % (600 / TIMER_PERIOD) <= (300 / TIMER_PERIOD)) {
        //
        //LCD_DisplayString(1, dispMe);
        //
        //} else {
        //
        //LCD_ClearScreen();
    //}
//}

char* getModeFromEnum(unsigned char mode_enum) {
    
    switch (mode_enum) {
        
        case FREE_PLAY:
        return "Free Play";
        
        case RECORD:
        return "Record";
        
        case PLAY_BACK:
        return "Play Back";
        
        default:
        return "getModeFromEnum() broken...";
    }
}

unsigned char cycleLeft(unsigned char currentMode) {
    
    switch (currentMode) {
        
        case FREE_PLAY:
        return PLAY_BACK;
        
        case PLAY_BACK:
        return RECORD;
        
        case RECORD:
        default:
        return FREE_PLAY;
        
    }
}

unsigned char tick_Menu(unsigned char state) {
    
    static unsigned char i;
    static unsigned char currentSelection;
    
    //Transitions
    switch (state) {
        
        case MENU_START:
            mode = MENU;
            i = 0;
            currentSelection = FREE_PLAY;
            task_in_progress = false;
            state = MENU_DISP;
            
            //LCD_DisplayString(1, getModeFromEnum(currentSelection));
            LCD_DisplayLine(true, getModeFromEnum(currentSelection));
            //LCD_DisplayString(1, "Menu START");
        
            break;
        
        case MENU_DISP:
        
            //LCD_DisplayString(1, "STARTED in menu disp");
        
            if (lr == NO_JOY && !getButtonInput(true)) {
                
                state = MENU_DISP;
            }
            
            else if (!getButtonInput(true)
            && (lr == DOWN_LO || lr == DOWN_HI)) {
                
                state = MENU_LEFT_WAIT;
                i = 0;
                currentSelection = cycleLeft(currentSelection);
            }
            
            else if (getButtonInput(true)) {
                
                state = MENU_MODE_PRESS;
            }
            
            break;
        
        case MENU_LEFT_WAIT:
        
            if (getButtonInput(true)) {
            
                state = MENU_MODE_PRESS;
            
                } else if (lr == NO_JOY) {
            
                state = MENU_DISP;
                //i = 0;
                //LCD_DisplayString(1, getModeFromEnum(currentSelection));
                LCD_DisplayLine(true, getModeFromEnum(currentSelection));
            
                } else if (lr == DOWN_LO || lr == DOWN_HI) {
            
                state = ((i >= 50) ? MENU_LEFT_ACTIVE : MENU_LEFT_WAIT);
            
            }
            break;
        
        case MENU_LEFT_ACTIVE:
        
            state = MENU_LEFT_WAIT;
            i = 0;
            break;
        
        case MENU_MODE_PRESS:
        
            state = (getButtonInput(true) ? MENU_MODE_PRESS : MENU_MODE_RUNNING);
            break;
        
        case MENU_MODE_RUNNING:
        
            //if (getButtonInput(true)) {
            //
                //state = MENU_DISP;
                ////i = 0;
                //LCD_DisplayString(1, getModeFromEnum(currentSelection));
            //} else {
            //
                //state = MENU_MODE_RELEASE;
            //}
            //state = (task_in_progress ? MENU_MODE_RUNNING : MENU_DISP);
            
            if (task_in_progress) {
                state = MENU_MODE_RUNNING;
            } else {
                
                state = MENU_DISP;
                currentSelection = FREE_PLAY;
                //LCD_DisplayString(1, getModeFromEnum(currentSelection));
                LCD_DisplayLine(true, getModeFromEnum(currentSelection));
                task_in_progress = false;
            }
            
            break;
        
    }
    
    //Actions
    switch (state) {
        case MENU_START:
            break;
        
        case MENU_DISP:
            //disp_flashing(getModeFromEnum(currentSelection), i);
            //i++;
            break;
        
        case MENU_LEFT_WAIT:
            //i++;
            //LCD_DisplayString(1, getModeFromEnum(currentSelection));
            break;
        
        case MENU_LEFT_ACTIVE:
            currentSelection = cycleLeft(currentSelection);
            //LCD_DisplayString(1, getModeFromEnum(currentSelection));
            LCD_DisplayLine(true, getModeFromEnum(currentSelection));
            break;
        
        case MENU_MODE_PRESS:
            mode = currentSelection;
            task_in_progress = true;
            break;
        
        case MENU_MODE_RUNNING:
        default:
            break;
        
    }
    
    return state;
}




#endif // MENU_TASK_H
