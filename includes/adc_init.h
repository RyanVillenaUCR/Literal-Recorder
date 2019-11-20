//Header file compiled by Ryan Villena,
//but function implementation provided by IEEE @ UCR.

#ifndef ADC_INIT_H
#define ADC_INIT_H

#include <avr/io.h>

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: setting this bit enables analog-to-digital conversion.
    // ADSC: setting this bit starts the first conversion.
    // ADATE: setting this bit enables auto-triggering. Since we are
    //        in Free Running Mode, a new conversion will trigger whenever
    //        the previous conversion completes.
}

#endif // ADC_INIT_H