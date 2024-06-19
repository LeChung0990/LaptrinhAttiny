/*
 * CompareMatch.c
 *
 * Created: 7/23/2023 12:49:45 AM
 * Author : admin
 */ 
/*CTC Mode != PWM Mode
Because:
1. PWM generates variable width pulses
2. CTC generates pulses but the width is always 50%

Choose Pin OC0A is pulse output
Generate pulse frequency 1KHZ, period 1ms
So set TCNT0 couter in 500us
F_OC0A = F_CPU/ (2*PRESCALE)*(1+OCR0A)
-> 1000 = 8000000/(2*8)*(1+OCR0A)--> OCR0A = 499 (no)
Choose prescale is 64 --> OCR0A = 61.5
*/
#define F_CPU 8000000UL
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>
int main(void)
{
	DDRB |= (1<<2); //PB2 IS OUTPUT
	while(1)
	{
		OCR0A = 249;
		/*CTC MODE, TOGGLE ON COMPARE MATCH, PRESCALE/64*/
		TCCR0B |= (1<<CS01)|(1<<CS00);
		TCCR0A |= (1<<COM0A0)|(1<<WGM01);	
		while((TIFR0&(1<<OCF0A)) == 0); /* MONITOR OCF0A FLAG*/
		TIFR0 = (1<<OCF0A); /*CLEAR OCF0A BY WRITING 1*/
		OCR0A = 124;
		/*CTC MODE, CLEAR ON COMPARE MATCH, PRESCALE/64
		MUST SET CLEAR OC0A BECAUSE WHILE TON PB2 = 1 SHOULD PULL DOWN 0
		*/
		TCCR0B |= (1<<CS01)|(1<<CS00);
		//TCCR0A |= (1<<COM0A1)|(1<<WGM01)|(0<<COM0A0);	
		TCCR0A = 0x82;
		while((TIFR0&(1<<OCF0A)) == 0); /* MONITOR OCF0A FLAG*/
		TIFR0 = (1<<OCF0A); /*CLEAR OCF0A BY WRITING 1*/
	}
}