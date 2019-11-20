
/********************************************************************************/
/* This H file is for declaring constants and functions that other tasks use.   */
/* Things internal to the main() function are not declared here.                */
/********************************************************************************/

#ifndef LITERAL_RECORDER_1_H
#define LITERAL_RECORDER_1_H

//#include "mybit.h"

bool getButtonInput(bool left);

const unsigned char MIC_PIN = 1;
const unsigned char JOY_UD_PIN = 3;
const unsigned char JOY_LR_PIN = 0;
const unsigned char RIGHT_BUTTON_PIN = 2;
const unsigned char LEFT_BUTTON_PIN = 4;

//const unsigned char TIMER_PERIOD = 10;

//Only MENU writes to this
unsigned char mode;

//MENU clears this, other tasks set it
bool task_in_progress;

//Only ADC_INPUT writes to these
bool mic;
unsigned char lr_actual;
unsigned char lr;
unsigned char ud;

typedef struct _task {
    int state; // Current state of the task
    unsigned long period; // Rate at which the task should tick
    unsigned long elapsedTime; // Time since task's previous tick
    int (*TickFct)(int); // Function to call for task's tick
} task;

const unsigned char tasksNum = 5;
task tasks[5];

const unsigned short tasksPeriodGCD = 10;

enum MODES {
    MENU,
    FREE_PLAY,
    RECORD, PLAY_BACK
};

unsigned char recordedSong[64];

#endif // LITERAL_RECORDER_1_H