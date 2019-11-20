//This header file provided entirely by IEEE @ UCR.

#ifndef __scheduler_h__
#define __scheduler_h__

#include <stdbool.h>

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b) {
    
    unsigned long int c;
    
    while (true) {
        
        c = a % b;
        
        if (c == 0) return b;
        
        a = b;
        b = c;
    }
    
    return 0;
}
//--------End find GCD function ----------------------------------------------

////--------Task scheduler data structure---------------------------------------
//// Struct for Tasks represent a running process in our simple real-time operating system.
//typedef struct _task {
    ///*Tasks should have members that include: state, period,
    //a measurement of elapsed time, and a function pointer.*/
    //signed char state; //Task's current state
    //unsigned long int period; //Task period
    //unsigned long int elapsedTime; //Time elapsed since last task tick
    //int (*TickFct)(int); //Task tick function
//} task;
//
////--------End Task scheduler data structure-----------------------------------
//
////--------Shared Variables----------------------------------------------------
//unsigned char SM2_output = 0x00;
//unsigned char SM3_output = 0x00;
//unsigned char pause = 0;
//
////--------End Shared Variables------------------------------------------------
//
////--------User defined FSMs---------------------------------------------------
////Enumeration of states.
//enum SM1_States { SM1_wait, SM1_press, SM1_release };
//
//// Monitors button connected to PA0.
//// When button is pressed, shared variable "pause" is toggled.
//int SMTick1(int state) {
//
    //// Local Variables
    //unsigned char press = ~PINA & 0x01;
//
    ////State machine transitions
    //switch (state) {
        //case SM1_wait: 	if (press == 0x01) {	// Wait for button press
            //state = SM1_press;
        //}
        //break;
//
        //case SM1_press:	state = SM1_release;
        //break;
//
        //case SM1_release:	if (press == 0x00) {	// Wait for button release
            //state = SM1_wait;
        //}
        //break;
//
        //default:		state = SM1_wait; // default: Initial state
        //break;
    //}
//
    ////State machine actions
    //switch(state) {
        //case SM1_wait:	break;
//
        //case SM1_press:	pause = (pause == 0) ? 1 : 0; // toggle pause
        //break;
//
        //case SM1_release:	break;
//
        //default:		break;
    //}
//
    //return state;
//}
//
////Enumeration of states.
//enum SM2_States { SM2_wait, SM2_blink };
//
//// If paused: Do NOT toggle LED connected to PB0
//// If unpaused: toggle LED connected to PB0
//int SMTick2(int state) {
//
    ////State machine transitions
    //switch (state) {
        //case SM2_wait:	if (pause == 0) {	// If unpaused, go to blink state
            //state = SM2_blink;
        //}
        //break;
//
        //case SM2_blink:	if (pause == 1) {	// If paused, go to wait state
            //state = SM2_wait;
        //}
        //break;
//
        //default:		state = SM2_wait;
        //break;
    //}
//
    ////State machine actions
    //switch(state) {
        //case SM2_wait:	break;
//
        //case SM2_blink:	SM2_output = (SM2_output == 0x00) ? 0x01 : 0x00; //toggle LED
        //break;
//
        //default:		break;
    //}
//
    //return state;
//}
//
////Enumeration of states.
//enum SM3_States { SM3_wait, SM3_blink };
//
//// If paused: Do NOT toggle LED connected to PB1
//// If unpaused: toggle LED connected to PB1
//int SMTick3(int state) {
    ////State machine transitions
    //switch (state) {
        //case SM3_wait:	if (pause == 0) {	// If unpaused, go to blink state
            //state = SM3_blink;
        //}
        //break;
//
        //case SM3_blink:	if (pause == 1) {	// If paused, go to wait state
            //state = SM3_wait;
        //}
        //break;
//
        //default:		state = SM3_wait;
        //break;
    //}
//
    ////State machine actions
    //switch(state) {
        //case SM3_wait:	break;
//
        //case SM3_blink:	SM3_output = (SM3_output == 0x00) ? 0x02 : 0x00; //toggle LED
        //break;
//
        //default:		break;
    //}
//
    //return state;
//}
//
//
////Enumeration of states.
//enum SM4_States { SM4_display };
//
//// Combine blinking LED outputs from SM2 and SM3, and output on PORTB
//int SMTick4(int state) {
    //// Local Variables
//
    //unsigned char output;
//
    ////State machine transitions
    //switch (state) {
        //case SM4_display:	break;
//
        //default:		state = SM4_display;
        //break;
    //}
//
    ////State machine actions
    //switch(state) {
        //case SM4_display:	output = SM2_output | SM3_output; // write shared outputs
        //// to local variables
        //break;
//
        //default:		break;
    //}
//
    //PORTB = output;	// Write combined, shared output variables to PORTB
//
    //return state;
//}
//
//// --------END User defined FSMs-----------------------------------------------
#endif