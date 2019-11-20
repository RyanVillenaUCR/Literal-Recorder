/*
 * LiteralRecorder1.c
 *
 * Created: 6/6/2018 5:49:26 AM
 *  Author: getaf
 */ 

#ifndef LITERAL_RECORDER_1_C
#define LITERAL_RECORDER_1_C

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <assert.h>
#include <stdbool.h>
#include "LiteralRecorder1.h"
#include "joystick_io.h"
#include "lcd_io.h"
#include "timer.h"
#include "bit.h"
#include "pwm.h"
#include "adc_init.h"
#include "mybit.h"
#include "mic.h"
#include "note.h"
#include "menu_task.h"
#include "ADC_in_task.h"
#include "record_task.h"
#include "playback_task.h"
#include "freeplay_task.h"

bool getButtonInput(bool left) {

    //return ~(MyGetBit(PINA, RIGHT_BUTTON_PIN));
    if (left) {
        return !(MyGetBit(PINA, LEFT_BUTTON_PIN));
    } else {
        return !(MyGetBit(PINA, RIGHT_BUTTON_PIN));
    }
}





void init_tasks() {
    
    unsigned char i = 0;
    
    tasks[i].state = MENU_START;
    tasks[i].period = 200;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &tick_Menu;
    
    i++;
    
    tasks[i].state = ADC_IN_START;
    tasks[i].period = 20;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &tick_ADC_in;
    
    i++;
    
    tasks[i].state = FREEPLAY_START;
    tasks[i].period = 10;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &tick_Freeplay;
    
    i++;
    
    tasks[i].state = RECORD_START;
    tasks[i].period = 10;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &tick_Record;
    
    i++;
    
    tasks[i].state = PLAYBACK_START;
    tasks[i].period = 10;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &tick_Playback;
    
    i++;
}    

void init_vars() {
    
    //ADC input (need PA0:PA3)
    DDRA = 0x00;
    PORTA = 0xFF;
    
    //LCD display output
    DDRC = 0xFF;
    PORTC = 0x00;
    
    //PD0-PD3: Button input
    //PD6-PD7: LCD output
    DDRD = 0xC0;    //1100 0000
    PORTD = 0x3F;   //0011 1111
    
    //PB6: Speaker output
    DDRB = 0xFF;
    PORTB = 0x00;
    
    ADC_init();
    
    LCD_init();

    PWM_on();
    
    //Initialize timer
    //TimerSet(TIMER_PERIOD);
    TimerSet(tasksPeriodGCD);
    TimerOn();
    
    buildSymbols();

    //initialize recordedSong
    unsigned char temp[64];
    memset((void*) temp, SHH_INDEX, 64);
    //eeprom_update_block((const void*) temp, (void*) 12, 64);
        
}

void scheduler() {
        
    for (unsigned char i = 0; i < tasksNum; i++) {
            
        if (tasks[i].elapsedTime >= tasks[i].period) {
                
            // Task is ready to tick, so call its tick function
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0; // Reset the elapsed time
                
        }
        tasks[i].elapsedTime += tasksPeriodGCD;
        //tasks[i].elapsedTime += TIMER_PERIOD;
    }
}

int main(void) {
    
    
    init_vars();
    init_tasks();
    
    //LCD_DisplayString(1, "hello world!");
        
    while (true) {
        
        scheduler();

        while (!TimerFlag) {}
        TimerFlag = false;
    }
        
    return 0;
}

#endif // LITERAL_RECORDER_1_C
