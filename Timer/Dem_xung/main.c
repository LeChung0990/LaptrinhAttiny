/*
 * Dem_xung.c
 *
 * Created: 9/27/2023 11:42:14 AM
 * Author : admin
 */ 
#define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define but_fre	2

int flag = 0;
int dem = 0;
long fre;
uint8_t start = 0;
int main(void)
{
	DDRA &= ~(1<<3);
	PORTA |= (1<<3);
	DDRB &= ~(1<<but_fre);
	PORTB |= (1<<but_fre);
	/* Reset Timer/Counter 0 */
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;
	             	
	TCCR0B = 0x07;	//T0. rising 
	TIMSK0 = 0x01;	// Overflow interrupt enable
	cli();

    while (1) 
    {
		/*Push Button PB2 to start counter clock signal */
		if(!(PINB & (1<<but_fre))){
			_delay_ms(200);
			while(!(PINB & (1<<but_fre)));
			start = 1;
		}
		
		while(start == 1){
			sei();
			TCNT0 = 0;
			flag = 0;
			_delay_ms(1000);
			dem = TCNT0;
			cli();	//tat ngat
			fre =  (long)flag;
			fre <<= 8;
			fre += (long)dem;
			start = 0;
		}
    }
}

ISR(TIM0_OVF_vect)
{
	flag++;
}
