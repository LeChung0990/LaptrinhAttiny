#define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lib.h"

#define	led		6	//LED A6
#define button	2	//BUTTON B2

void main(void)
{
    DDRB &= ~(1 << button);
    DDRA |= (1 << led);

    // BUTTON = INPUT PULLUP
    PORTB |= (1 << button);
    // LED OFF
    PORTA &= ~(1 << led);
    tinhtoan();
    while (1) 
    {
        /*Nhay Led bat tat 1s*/
		PORTA |= (1<<led);
		_delay_ms(1000);
		PORTA &= ~(1<<led);
		_delay_ms(1000);
        //CHECK BUTTON
		if((PORTB & (1<< button))==0)
		{
			_delay_ms(100);
			if((PORTB & (1<< button))==0)
			{
				PORTB ^= (1<<led);
			}
		}
    }
}

