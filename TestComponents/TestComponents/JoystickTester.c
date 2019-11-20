//#include <avr/io.h>
//#include <stdbool.h>
//#include "joystick_io.h"
//#include "adc_init.h"
//#include "timer.h"
//#include "lcd_io.h"
////#include <avr/delay.h>
//#include <util/delay.h>
////#include <avr/eeprom.h>
//
////change as necessary for your board configuration
//const unsigned char UD_PORT = 0;
//const unsigned char LR_PORT = 3;
//
//void initialize() {
    //
    ////LCD DDR's
    //DDRC = DDRD = 0xFF;
    //PORTC = PORTD = 0x00;
    //
    ////Joystick DDR's
    //DDRA = 0x00;
    //PORTA = 0xFF;
    //
    //ADC_init();
    //LCD_init();
    //TimerSet(200);
    //TimerOn();
    //
//}
//
//int main() {
    //
    //initialize();
    //
    //while (true) {
        //
        ////Receives Center, up_lo, UP_HI, down_lo, or DOWN_HI
        //unsigned char UD_input = getJoystickDiscrete(UD_PORT);
        //_delay_ms(20);
        //unsigned char LR_input = getJoystickDiscrete(LR_PORT);
//
        //LCD_displayTwoLines(msgFromJoyEnum(UD_input), msgFromJoyEnum(LR_input));
        ////LCD_displayTwoLines("test disp 1!", "0123456789ABCDE");
        //
        ////LCD_DisplayString(1, msgFromJoyEnum(UD_input));
        ////LCD_DisplayString(1, msgFromJoyEnum(LR_input));
        //
        ////Can't print every clock cycle, so wait for a delay
        //while (!TimerFlag) {}
        //TimerFlag = false;
    //}
    //
    //return 0;
//}