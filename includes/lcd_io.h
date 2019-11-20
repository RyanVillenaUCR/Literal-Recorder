//This header file written by Ryan Villena.
//Some functionality original. Some provided by IEEE @ UCR.
//Each function herein is properly credited.


#ifndef LCD_IO_H
#define LCD_IO_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
//#include <util/delay.h>
#include "concat.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

//Provided by IEEE @ UCR
void delay_ms(int miliSec) //for 8 Mhz crystal
{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
    {
        asm("nop");
    }
}

//Provided by IEEE @ UCR
void LCD_WriteCommand (unsigned char Command) {
    CLR_BIT(CONTROL_BUS,RS);
    DATA_BUS = Command;
    SET_BIT(CONTROL_BUS,E);
    asm("nop");
    CLR_BIT(CONTROL_BUS,E);
    delay_ms(2); // ClearScreen requires 1.52ms to execute
}

//Provided by IEEE @ UCR
void LCD_ClearScreen(void) {
    LCD_WriteCommand(0x01);
}

//Provided by IEEE @ UCR
void LCD_init(void) {

    //wait for 100 ms.
    //delay_ms(100);
    delay_ms(100);
    LCD_WriteCommand(0x38);
    LCD_WriteCommand(0x06);
    LCD_WriteCommand(0x0f);
    LCD_WriteCommand(0x01);
    delay_ms(10);
}

//Provided by IEEE @ UCR
void LCD_WriteData(unsigned char Data) {
    SET_BIT(CONTROL_BUS,RS);
    DATA_BUS = Data;
    SET_BIT(CONTROL_BUS,E);
    asm("nop");
    CLR_BIT(CONTROL_BUS,E);
    delay_ms(1);
}

//Provided by IEEE @ UCR
void LCD_Cursor(unsigned char column) {
    
    // 16x1 LCD: column < 9
    if ( column < 17 ) { 
        
    // 16x2 LCD: column < 17
    LCD_WriteCommand(0x80 + column - 1);
    
    } else {
        
        // 16x1 LCD: column - 1
        LCD_WriteCommand(0xB8 + column - 9);	
        // 16x2 LCD: column - 9
    }
}

//Provided by IEEE @ UCR
void LCD_DisplayString( unsigned char column, const char* string) {
    //void LCD_DisplayString( unsigned char column, const unsigned char* string) {
    LCD_ClearScreen();
    unsigned char c = column;
    while(*string) {
        LCD_Cursor(c++);
        LCD_WriteData(*string++);
    }
}

//Written by Ryan Villena
void LCD_ClearLine(bool firstLine) {
    
    for (unsigned char i = 0; i < 16; i++) {
        
        if (firstLine) {
            
            LCD_Cursor(i + 1);
            
        } else {
            
            LCD_Cursor(16 + i + 1);
        }

        LCD_WriteData(' ');
    }
}

//Written by Ryan Villena
void LCD_DisplayLine(bool firstLine, const char* string) {
    
    LCD_ClearLine(firstLine);
    
    for (unsigned char i = 1; i < 17; i++) {
        
        if (string[i - 1] == '\0') break;
        
        unsigned char cursor = (firstLine ? i : i + 16);
        LCD_Cursor(cursor);
        LCD_WriteData(string[i - 1]);
    }
    
    //unsigned char cursor = 16 + 1;
    //while (*string && cursor < 32 + 1) {
        //LCD_Cursor(cursor);
        //LCD_WriteData(*string);
        //
        //cursor++;
        //string++;
    //}
}

//Written by Ryan Villena
void LCD_DisplayDigits(bool firstLine, unsigned char number) {
    
    LCD_Cursor(firstLine ? 1 : 17);
    LCD_WriteData('0' + (number / 100));
    
    LCD_Cursor(firstLine ? 2 : 18);
    LCD_WriteData('0' + ((number % 100) / 10));
    
    LCD_Cursor(firstLine ? 3 : 19);
    LCD_WriteData('0' + (number % 10));
    
    //if (number < 10) {
        //
        //LCD_Cursor(1);
        //LCD_WriteData('0');
        //
        //LCD_Cursor(2);
        //LCD_WriteData('0');
        //
        //LCD_Cursor(3);
        //LCD_WriteData('0' + number);
        //
    //} else if (number < 100) {
        //
        //LCD_Cursor(1);
        //LCD_WriteData('0');
        //
        //LCD_Cursor(2);
        //LCD_WriteData('0' + (number / 10));
        //
    //} else {
        //
        //
    //}
}

//Written by Ryan Villena
//both cstrings no more than 16 chars
void LCD_displayTwoLines(char* line1, char* line2) {
    
    //Check that line1 and line2 are valid
    if (strlen(line1) > 16) {
        
        LCD_DisplayString(1, "line1 too big!");
        return;
        
        } else if (strlen(line2) > 16) {
        
        LCD_DisplayString(1, "line2 too big!");
        return;
    }
    
    //If you're here, then line1 and line2 are safe
    size_t slack = 16 - strlen(line1);
    
    if (slack > 0) {
        
        slack++;    //idk why but this has to be here
        
        //Prepare slack_cstr
        char* slack_cstr = malloc(slack);
        memset(slack_cstr, ' ', slack);
        slack_cstr[slack - 1] = '\0';
        
        char* temp1 = concat_and_alloc(line1, slack_cstr);
        char* temp2 = concat_and_alloc(temp1, line2);
        
        LCD_DisplayString(1, temp2);
        
        free(temp1);
        free(temp2);
        free(slack_cstr);
        
        } else {
        
        char* temp = concat_and_alloc(line1, line2);
        
        LCD_DisplayString(1, temp);
        
        free(temp);
    }
}

//Provided by HackProjects.com
//Special thanks to the author at:
//https://hackprojects.wordpress.com/forum/avr/display-custom-character-on-16x2-lcd-to-avr-microcontroller-atmega-8/
void LCDBuildChar(unsigned char loc, const unsigned char *p) {
    
    if (loc < 8) {
        
        LCD_WriteCommand(0x40 + (8 * loc)); //Write to CGRAM
        
        //Write the character pattern to CGRAM
        for (unsigned char i = 0; i < 8; i++) {
            
            LCD_WriteData(p[i]);
        }
    }
    
    LCD_WriteCommand(0x80); //shift back to DDRAM location 0
    
}

//Written by Ryan Villena
void LCD_WriteSubstring(unsigned char column, char* begin, size_t n) {
    
    for (char* i = begin; i < begin + n; i++) {
        
        if (*i == '\0') break;
        
        LCD_Cursor(column);
        LCD_WriteData(*i);
        
        column++;
    }
}

#endif // LCD_IO_H
