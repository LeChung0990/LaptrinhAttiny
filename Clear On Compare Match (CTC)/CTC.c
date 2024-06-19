/*CTC Mode != PWM Mode
Because:
1. PWM generates variable width pulses
2. CTC generates pulses but the width is always 50%

*/
/* Choose Pin OC0A is pulse output
Generate pulse frequency 1KHZ, period 1ms
So set TCNT0 couter in 500us 
F_OC0A = F_CPU/ (2*PRESCALE)*(1+OCR0A)
-> 1000 = 8000000/(2*8)*(1+OCR0A)--> OCR0A = 499 (no)
Choose prescale is 64 --> OCR0A = 61.5
*/

#define F_CPU 8000000UL
#include <avr/io.h>

int main()
{
	DDRB |= (1<<2); //PB2 IS OUTPUT
	/*CTC MODE, TOGGLE ON COMPARE MATCH, PRESCALE/64*/
	TCCR0B |= (1<<CS01)|(1<<CS00);
	TCCR0A |= (1<<COM0A0)|(1<<WGM01);		
	OCR0A = 61; //COMPARE VALUE
	while(1){}
}
----RESULT: Generate pulse frequency is 1008Hz-----
----IF OCR0A = 62 generate pulse frequency is 992 Hz

####### CODE 2 #######
/*
Generate pulse Ton 1ms, Toff 2ms
T_OC0A first = Toff = PRESCALE*(1+OCR0A) / F_CPU = 64*(1+61)/ 8000000 = 496us

-> Toff = 2ms -> OCR0A = 249
-> Ton = 1ms -> OCR0A = 124
*/
int main()
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
----RESULT: Generate pulse frequency is Ton 1ms, Toff 2ms-----