/*
 * FastPWMTimer0.c
 *
 * Created: 7/22/2023 12:53:51 AM
 * Author : admin
 */ 
/* PROGRAM USING TIMER0 FAST PWM MODE
1. Set mode FAST PWM by bit WGM02,WGM01,WGM00=1
	WGM02 in TCCR0B register, WGM01 : WGM00 in TCCR0A
2. Set Compare Output A Mode:
	Non-inv : COM0A1 = 1, COM0A0 = 0 in TCCR0A
3. Clock select bit
	Clk/8 : CS01 = 1 in TCCR0B
4. Pin OC0A is PB2 (5)
*/

#define F_CPU 8000000UL
#include <avr/io.h>
//#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void PWM_Init();
uint8_t low, high;
uint16_t ADC_Value;

void PWM_Init()
{  
	DDRB |= (1<<DDB2);
	//TCCR0A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0A1);
	//TCCR0B |= (1<<CS01);
	TCCR0A |= (1<<WGM00)|(1<<COM0A1);
	TCCR0B |= (1<<CS00);
	OCR0A = 0;
	
}
int main(void)
{
	
	PWM_Init(); 
	sei();
	//DDRA &= ~(1<<2);
	
	ADMUX = (1 << MUX1); //pa2
	ADMUX &= ~(1 << REFS0) | ~(1 << REFS1); //Vcc used as analog reference
	ADCSRA |= (1 << ADEN)|(1<<ADIE)|(1<<ADIF);                  //ADC Enable
	ADCSRA |= (1 << ADPS2)|(1<<ADPS1);    // ADC PRESCALER /64
	ADCSRA |= (1 << ADSC);                //Start Conversion
	
	while(1)
	{
	}
}
ISR (ADC_vect)
{
	OCR0A = ADCW/4;
	ADCSRA |= (1 << ADSC);                //Start Conversion
}