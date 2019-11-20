//This entire testing file written by Ryan Villena.


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include "joystick_io.h"
#include "lcd_io.h"
#include "timer.h"
#include "bit.h"
#include "pwm.h"
#include "mic.h"
#include "mybit.h"
#include "adc_init.h"
#include "note.h"

char EEMEM msg_inEEPROM[10 + 1];


//void printBasedOnChar(unsigned char joyInput) {
    //
    //LCD_DisplayString(1, msgFromJoyEnum(joyInput));
//}

void testJoystick() {
                
    while (true) {
        
        while (!TimerFlag) {}
        TimerFlag = false;
                
        unsigned char joystickInput = getJoystickDiscrete(3);
        LCD_DisplayString(1, msgFromJoyEnum(joystickInput));
    }
}

void testTones() {
    
    set_PWM(261.63);
    
    //PWM_on();
    
    //while(true) {
        //
        ////set_PWM(261.63);
    //}
}

void testMic() {
        
    while (1) {
        
        bool isTone = getMicBinary(1);
        
        if (isTone) {
            
            LCD_DisplayString(1, "AAAAAA");
        } else {
            
            LCD_DisplayString(1, "shh");
        }
        
        
        while (!TimerFlag) {}
        TimerFlag = false;
    }
}

void testButtonsFromPortD() {
    
    //DDRD = 0xC0;    //1100 0000
    //PORTD = 0x3F;   //0011 1111
    
    
    while (1) {
        unsigned char lowerNibble = ~PIND & 0x0F;
        
        switch (lowerNibble) {
            case 0x00:
                LCD_DisplayString(1, "grabbing 0");
                break;
                
            case 0x01:
                LCD_DisplayString(1, "grabbing 1");
                break;
                
            case 0x02:
                LCD_DisplayString(1, "grabbing 2");
                break;
                
            case 0x03:
                LCD_DisplayString(1, "grabbing 3");
                break;
                
            case 0x04:
                LCD_DisplayString(1, "grabbing 4");
                break;
                
            case 0x05:
                LCD_DisplayString(1, "grabbing 5");
                break;
                
            case 0x06:
                LCD_DisplayString(1, "grabbing 6");
                break;
                
            case 0x07:
                LCD_DisplayString(1, "grabbing 7");
                break;
                
            case 0x08:
                LCD_DisplayString(1, "grabbing 8");
                break;
                
            case 0x09:
                LCD_DisplayString(1, "grabbing 9");
                break;
                
            case 0x0A:
                LCD_DisplayString(1, "grabbing 10");
                break;
                
            case 0x0B:
                LCD_DisplayString(1, "grabbing 11");
                break;
                
            case 0x0C:
                LCD_DisplayString(1, "grabbing 12");
                break;
                
            case 0x0D:
                LCD_DisplayString(1, "grabbing 13");
                break;
                
            case 0x0E:
                LCD_DisplayString(1, "grabbing 14");
                break;
                
            case 0x0F:
                LCD_DisplayString(1, "grabbing 15");
                break;
                
            default:
                LCD_DisplayString(1, "grabbing something else");
                break;
        }
        
        //if (lowerNibble == 0x00) {
            //LCD_DisplayString(1, "grabbing 0");
        //} else if (lowerNibble == 0x03) {
            //LCD_DisplayString(1, "grabbing 3");
        //} else if (lowerNibble == 0x0F) {
            //LCD_DisplayString(1, "grabbing 15");
        //} else if (lowerNibble == 0x01) {
            //LCD_DisplayString(1, "grabbing 1");
        //}  else {
            //LCD_DisplayString(1, "grabbing something else");
        //}
        
        while (!TimerFlag) {}
        TimerFlag = false;
    }    

}

void test2DJoystick() {
    
    while(true) {
        
        unsigned char joy_UD = getJoystickDiscrete(0); //PA0 is UD
        _delay_ms(1);
        unsigned char joy_LR = getJoystickDiscrete(3); //PA3 is LR
        
        char* str_UD = msgFromJoyEnum(joy_UD);
        char* str_LR = msgFromJoyEnum(joy_LR);
        
        LCD_displayTwoLines(str_UD, str_LR);
        
        while(!TimerFlag) {}
        TimerFlag = false;
        
    }
}

void testStrings() {
    
    char* hello = "ryan and ";
    char* world = "jill!";
    
    //char* hello = "0123456789ABCDEF";
    //char* world = "0123456789ABCDEF";
    
    //char* hello_world = safe_concat(hello, world);
    //
    //LCD_DisplayString(1, hello_world);
    //
    //free(hello_world);
    
    LCD_displayTwoLines(hello, world);
}

//some notes about EEPROM blocks:
    //you must declare the EEPROM block as a global variable
        //this is also the only time you can static-ly allocate it memory
        //you may not, however, initialize it here
    //use eeprom_read_block() to read from it
    //use eeprom_update_block() to write to it
    //eeprom does NOT retain its contents between reprogramming
        //only through power on/off
    //this eeprom has 4096 bytes of memory
    //eeprom updates are FINITE!! you only get 100K lifetime
        //please please please be efficient with them!!
        //seriously this shit costs money
        //no limit on reads, only writes
    
        
void test_both_EEPROM() {
    
    //Update
    char* msg = " test6 ";
    //eeprom_update_block( (const void*) StringOfData, (void*) 12, 10);
    eeprom_update_block( (const void*) msg, (void*) msg_inEEPROM, 10);
    
    //LCD_DisplayString(1, "Updated \"msg\".");

    
    //Wait until a button is pressed
    //During this wait, power off/on!
    //while ((~PIND && 0x0F) == 0) {
    while (MyGetBit(PINA, 2)) {
        
        LCD_DisplayString(1, "Waiting...");
        
        while (!TimerFlag) {}
        TimerFlag = false;
    }
    
    //Display what you wrote before
    uint8_t arr_for_test_reading [10];
    //eeprom_read_block (( void *) arr_for_test_reading , ( const void *) 12, 10) ;
    eeprom_read_block((void*) arr_for_test_reading, (const void*) msg_inEEPROM, 10);
    
    LCD_DisplayString(1, arr_for_test_reading);
}

void testCustomChars() {
    
    unsigned char flat[]  = { 0x08, 0x08, 0x08, 0x0E, 0x0A, 0x0C, 0x08 };
    unsigned char sharp[] = { 0x0A, 0x0B, 0x1E, 0x0A, 0x0F, 0x1A, 0x0A };
        

    
    LCDBuildChar(1, sharp);
    LCD_Cursor(1);
    LCD_WriteData(1);
    
    LCD_WriteData(' ');
    
    LCDBuildChar(0, flat);
    LCD_Cursor(3);
    LCD_WriteData(0);
    
    //LCDBuildChar(3, flat);
    //LCD_WriteData(0x02);
    
    //unsigned char customChar[]  = {0x04,0x04,0x04,0x04,0x15,0x0e,0x04,0x00};
    //unsigned char customChar[] = { 0x08, 0x08, 0x08, 0x0E, 0x0A, 0x0C, 0x08 };
    //unsigned char customChar2[] = {0x04,0x0e,0x15,0x04,0x04,0x04,0x04,0x00};
    //unsigned char customChar3[] = {0x04,0x0e,0x15,0x04,0x04,0x04,0x04,0x00};
    //unsigned char customChar4[] = {0x1c,0x16,0x1d,0x01,0x1d,0x16,0x1c,0x00};
    //unsigned char customChar5[] = {0x1c,0x16,0x1d,0x01,0x1d,0x16,0x1c,0x00};
    //unsigned char customChar6[] = {0x1c,0x16,0x1d,0x01,0x1d,0x16,0x1c,0x00};
    //unsigned char customChar7[] = {0x1c,0x16,0x1d,0x01,0x1d,0x16,0x1c,0x00};
    //unsigned char customChar8[] = {0x1c,0x16,0x1d,0x01,0x1d,0x16,0x1c,0x00};
    
    //LCDBuildChar(0, customChar);
    //LCDBuildChar(1, customChar2); /* Build Character1 at position i */
    //LCDBuildChar(2, customChar3);
    //LCDBuildChar(3, customChar4);
    //LCDBuildChar(4, customChar5);
    //LCDBuildChar(5, customChar6);
    //LCDBuildChar(6, customChar7);
    //LCDBuildChar(7, customChar8);



    //LCD_WriteData(0x00);
    //LCD_WriteData(' ');
    //LCD_WriteData(0x01);
    //LCD_WriteData(' ');
    //LCD_WriteData(0x02);
    //LCD_WriteData(0x03);
    //LCD_WriteData(0x04);
    //LCD_WriteData(0x05);
    //LCD_WriteData(0x06);
    //LCD_WriteData(0x07);
}

void testPrintNote() {
    
    buildSymbols();
    
    for (unsigned char i = 0; i < LENGTH_ALLNOTENAMES; i++) {
        
        //Display note
        printNote(i, 3);
        
        //Wait for input to keep going
        while (!TimerFlag) {}
        TimerFlag = false;
    }        
    
    LCD_DisplayString(1, "Done!");
}

void testWaitPrompt() {
    
    LCD_DisplayString(1, "Waiting...");
    
    while (MyGetBit(PINA, 2)) {}
        
    LCD_DisplayString(1, "Done!");
}

void testWriteSubstring() {
    
    char* hw = "AMUSEMENT PARK";
    
    //LCD_WriteSubstring(3, hw + 3, 5);
    LCD_WriteSubstring(1, hw + 10, 5);
}

void testSwitch(unsigned char arg) {
    
    switch (arg) {
        
        case 7:
            LCD_DisplayString(1, "works fine");
            break;
            
        default:
            LCD_DisplayString(1, "wtf");
            break;
    }
}

void test_ScopeResolutionOperator() {
    
    enum VALUES { TEMP1, TEMP2};
        
    unsigned char arg = TEMP1;
    
    switch (arg) {
        
        case TEMP1:
            LCD_DisplayString(1, "VALUES::TEMP1");
            break;
            
        case TEMP2:
            LCD_DisplayString(1, "VALUES::TEMP2");
            break;
            
        default:
            LCD_DisplayString(1, "wait wtf");
            break;
    }
}

void testDisp2ndLine() {
    
    char* hw = "Hello world!";
    
    LCD_DisplayLine(true, hw);
    
    unsigned char i = 0;
    
    while (true) {
        
        switch (i) {
            
            case 0:
                //LCD_Display2ndLine("ryan");
                LCD_DisplayLine(false, "ryan");
                i++;
                break;
                
            case 1:
                //LCD_Display2ndLine("+");
                LCD_DisplayLine(false, "and");
                i++;
                break;
                
            case 2:
                //LCD_Display2ndLine("jill");
                LCD_DisplayLine(false, "jill");
                i = 0;
                break;
                
            default:
                //LCD_Display2ndLine("0123456789ABCDEF");
                LCD_DisplayLine(false, "0123456789ABCDEF");
                break;
            
        }
        
        while (!TimerFlag) {}
        TimerFlag = false;
    }
}

void testCasts() {
    
    //unsigned char number = '0' + 7;
    
    //char* p = &number;
    
    //LCD_DisplayLine(true, p);
    
    //LCD_Cursor(1);
    //LCD_WriteData(number);
    
    LCD_DisplayDigits(true, -1);
    
    LCD_DisplayDigits(false, 69);
}

int main(void) {
    
    DDRA = 0x00;
    PORTA = 0xFF;
    
    //DDRD = 0xFF;
    //PORTD = 0x00;
    DDRD = 0xC0;    //Set C6 & C7 for output to LED
    PORTD = 0x3F;   //Clear all others for buttons
    DDRC = 0xFF;
    PORTC = 0x00;
    
    DDRB = 0xFF;
    PORTB = 0x00;
    
    ADC_init();
    LCD_init();
    PWM_on();
    
    TimerSet(500);
    TimerOn();    
    
    
    
    //LCD_DisplayString(1, "Hello world!");
    
    //testJoystick();
    
    //testTones();
    
    //testMic();
    
    //testStrings();
    
    //test2DJoystick();
    
    //testButtonsFromPortD();

    //test_update_EEPROM();
    
    //test_read_EEPROM();
    
    //test_both_EEPROM();
    
    //testCustomChars();
    
    //testPrintNote();
    
    //testWaitPrompt();
    
    //testWriteSubstring();
    
    //testSwitch(7);
    
    //test_ScopeResolutionOperator();
    
    //testDisp2ndLine();
    
    testCasts();
}