/*
 * NgatNgoaiPCINT0.c
 *
 * Created: 7/23/2023 1:01:49 PM
 * Author : admin
 */ 
 #define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ngatPin  	3 	//PA3
#define LED 		2	//PA2
void Config_INT0()
{ 
	DDRA |= (1 << LED);			//A2 LED 
	PORTA |= (1 << LED);		//ON LED
	DDRA &= ~(1 << ngatPin);	//A3 BUTTON 
	PORTA |= ( 1<< ngatPin); 	//PULL UP
	
	MCUCR |= (1<<ISC01); 	//The falling edge of INT0
	PCMSK0 |= (1<<PCINT3);	//	PA3
	
	GIMSK |= (1<<PCIE0);
	GIMSK |= (1<<INT0);
	sei();   /* SREG |= (1 << 7) */
}
int main(void)
{
	Config_INT0();
	while(1)
	{
	}
}

ISR(PCINT0_vect)
{
	//*CHONG DOI PHIM*/
	if((PINA & (1 << ngatPin))==0)
	{
		_delay_ms(100);
		while((PINA & (1 << ngatPin))==0);
		PORTA ^= (1 << LED);	
	}
}
