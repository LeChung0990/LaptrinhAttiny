/*
1. PCMSK0 – Pin Change Mask Register 0 
PCINT7 PCINT6 PCINT5 PCINT4 PCINT3 PCINT2 PCINT1 PCINT0
A7		A6		A5		A4		A3		A2		A1		A0

2. MCUCR – MCU Control Register

ISC01 ISC00 Description
0 		0 	The low level of INT0 generates an interrupt request.
0 		1 	Any logical change on INT0 generates an interrupt request.
1 		0 	The falling edge of INT0 generates an interrupt request.
1 		1 	The rising edge of INT0 generates an interrupt request.

3. GIMSK – General Interrupt Mask Register
PCIE0: Pin Change Interrupt Enable 0
When the PCIE0 bit is set (one) and the I-bit in the Status Register (SREG) is set (one), pin
change interrupt 0 is enabled
*/
/*
Choose pin A7 is button, A6 is LED

*/
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(PCINT0_vect)
{
	while((PINA & (1<<7))==0);
	PORTA ^= (1<<6);
}
int main (void)
{
	DDRA |= (1<<6);		//A6 LED
	PORTA |= (1<<6);	//ON LED
	DDRA &= ~(1<<7);	//A7 BUTTON
	PORTA |= (1<<7); 	//PULL UP
	
	MCUCR |= (1<<ISC01); //The falling edge of INT0
	PCMSK0 |= (1<<PCINT7);
	
	GIMSK |= (1<<PCIE0);
	GIMSK |= (1<<INT0);
	
	sei();	//enable global interrupt <=> SREG |= (1<<7);
	
	while(1)
	{
		//sleep mode
	}
}
---Result : First, LED light if the button is pressed the LED will light up.
If the button is pressed again the LED will turn off