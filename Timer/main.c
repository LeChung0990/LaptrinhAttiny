/*
 * Clock.c
 *
 * Created: 7/20/2023 4:45:51 PM
 * Author : admin
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
//#include <util/delay_basic.h>
#include <util/delay.h>
int main(void)
{
	DDRB |= (1<<1);
    /* Replace with your application code */
    while (1) 
    {
		PORTB |=(1<<1);
		//_delay_loop_2(20000); //10ms
		_delay_ms(10);
		PORTB &= ~(1<<1);
		//_delay_loop_2(20000); //10mss
		_delay_ms(10);
    }
}

