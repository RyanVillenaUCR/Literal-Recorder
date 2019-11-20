/*
 * ADC_in_task.h
 *
 * Created: 6/8/2018 6:05:30 PM
 *  Author: getaf
 */ 

#ifndef ADC_IN_TASK_H
#define ADC_IN_TASK_H

#include "LiteralRecorder1.h"

extern unsigned char lr_actual;
extern unsigned char lr;
extern unsigned char ud;
extern bool mic;

enum STATES_ADC_IN {
    ADC_IN_START,
    ADC_IN_UD, ADC_IN_LR, ADC_IN_MIC
    };

unsigned char tick_ADC_in(unsigned char state) {
    
    //Transitions
    switch (state) {
        
        case ADC_IN_START:
            state = ADC_IN_UD;
            break;
            
        case ADC_IN_UD:
            state = ADC_IN_LR;
            break;
            
        case ADC_IN_LR:
            state = ADC_IN_MIC;
            break;
            
        case ADC_IN_MIC:
            state = ADC_IN_UD;
            break;
            
        
    }
    
    //Actions
    switch (state) {
        
        case ADC_IN_UD:
            ud = getJoystickDiscrete(JOY_UD_PIN);
            break;
            
        case ADC_IN_LR:
            lr = getJoystickDiscrete(JOY_LR_PIN);
            lr_actual = getJoystickValue(JOY_LR_PIN);
            break;
            
        case ADC_IN_MIC:
            //mic = getMicBinary(MIC_PIN);  //@TODO: test with actual mic
            mic = getButtonInput(false);
            break;
            
        default:
            break;
            
    }
    
    return state;
}

#endif // ADC_IN_TASK_H
