/*
 * NgatNgoaiPCINT0.c
 *
 * Created: 7/23/2023 1:01:49 PM
 * Author : admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "74HC595.h"
#define ngatINT0  2 //PB2
//uint8_t dem = 0;
void NgatINT0()
{ /*B2 IS INPUT , IS BUTTON ngatINT0, MODE PULLUP*/
	DDRB |= ~(1 << ngatINT0);
	PORTB |= (1 << ngatINT0);
	DDRA |= (1<<6);   //A6 LED is pin 7 of chip
	PORTA |= (1<<6);  //ON LED

	/*SET INT0 AS FALLING EDGE TRIGGER*/
	MCUCR |= (1 << ISC01);
	GIMSK |= (1 << INT0);
	sei();   /* SREG |= (1 << 7) */
}
int main(void)
{
	//DDRA |= (1<<6);		//A6 LED pin 7
	//PORTA |= (1<<6);	//ON LED
	//DDRA &= ~(1<<7);	//A7 BUTTON pin 6
	//PORTA |= (1<<7); 	//PULL UP
	////HC595_Init();
	//
	//MCUCR |= (1<<ISC01); //The falling edge of INT0
	//PCMSK0 |= (1<<PCINT7);
	//
	//GIMSK |= (1<<PCIE0);
	//GIMSK |= (1<<INT0);
	//
	//sei();	//enable global interrupt <=> SREG |= (1<<7);
	NgatINT0();
	while(1)
	{
		//sleep mode
			
		//if((PINA & (1<<3))==0)
		//{
			//_delay_ms(100);
			//while((PINA & (1<<3))==0){}
			//PORTA ^= (1<<7);		
		//}
		//HIENTHI(dem);
	}
}

//ISR(PCINT0_vect)
//{
	///* TAT LED 7 DOAN*/
	////HC595_PORT |= (1 << ST_CP);
	////HC595_PORT |= (1 << SH_CP);
	////HC595_PORT |= (1 << DATA);
	//
	///*CHONG DOI PHIM*/
	//if((PINA & (1<<7))==0)
	//{
		//_delay_ms(100);
		//while((PINA & (1<<7))==0);
		//PORTA ^= (1<<6);	
		////dem++;
	//}
//}
ISR(EXT_INT0_vect)
{
	if ((PINB & (1<< ngatINT0)) == 0)
	{
		_delay_ms(100);
		while ((PINB & (1 << ngatINT0)) == 0);
		PORTA ^= (1<<6);
	}
}