#define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "74HC595.h"
#include "ADC.h"

uint16_t ValueADC = 0;
float volt = 0;

int main(void)
{
	ADC_Init(7);
	HC595_Init();
	DDRA |= (1 << 2);
    /* Replace with your application code */
    while (1) 
    {	
		ValueADC = ADC_Read();
/* 		volt = (ValueADC * 5.0) / 1024.0;
		if(volt>2.5) PORTA |= (1 << 2); */
		/* else PORTA &= ~(1 << 2); */
/* 		FOUR7SEG(SEG_SELECT[0], SEG_MAP[(int)volt / 10]);
		FOUR7SEG(SEG_SELECT[1], SEG_MAP[((int)volt % 10)]|0x80);
		FOUR7SEG(SEG_SELECT[2], SEG_MAP[(int)(volt * 10) % 10]);
		FOUR7SEG(SEG_SELECT[3], SEG_MAP[(int)(volt * 100) % 10]); */
		
		FOUR7SEG(SEG_SELECT[0], SEG_MAP[(int)ValueADC / 1000]);
		FOUR7SEG(SEG_SELECT[1], SEG_MAP[(int)(ValueADC / 100) % 10]);
		FOUR7SEG(SEG_SELECT[2], SEG_MAP[(int)(ValueADC /10) % 10]);
		FOUR7SEG(SEG_SELECT[3], SEG_MAP[(int)(ValueADC / 1) % 10]);
    }
}


