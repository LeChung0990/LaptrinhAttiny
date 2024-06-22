/*
 * ADC.h
 *
 * Created: 7/12/2023 12:27:34 AM
 *  Author: admin
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
void ADC_Init(uint8_t channel); //Choose ADC & Enable ADC
uint16_t ADC_Read(void);        //Read ADC Voltage
/*------------
ADPS2	ADPS1	ADPS0	Division Factor
0 		0 		0 		2
0 		0 		1 		2
0		1 		0 		4
0 		1 		1 		8
1	 	0 		0 		16
1 		0 		1 		32
1 		1 		0 		64
1 		1 		1 		128
--------------*/
void ADC_Init(uint8_t channel)
{
	switch (channel)
	{
		case 0: ADMUX = ~(1 << MUX0); break;  //ADC0
		case 1: ADMUX = (1 << MUX0); break;
		case 2: ADMUX = (1 << MUX1); break;
		case 3: ADMUX = (1 << MUX0) | (1 << MUX1); break;
		case 4: ADMUX = (1 << MUX2); break;
		case 5: ADMUX = (1 << MUX0) | (1 << MUX2); break;
		case 6: ADMUX = (1 << MUX1) | (1 << MUX2); break;
		case 7: ADMUX = (1 << MUX0) | (1 << MUX1) | (1 << MUX2); break;
	}
	ADMUX &= ~(1 << REFS0); //Internal 1.1V voltage reference
	ADMUX |=  (1 << REFS1);
	ADCSRA |= (1 << ADEN);                  //ADC Enable
}

uint16_t ADC_Read(void)
{
	uint8_t low, high;
	uint16_t ADC_Value;

	ADCSRA |= (1 << ADSC);                //Start Conversion
	ADCSRA |= (1 << ADPS2)|(1<<ADPS0);    // ADC PRESCALER /32
	
	while ((ADCSRA & (1 << ADSC)) == 1);  // Wait conversion is complete

	low = ADCL;
	high = ADCH;
	ADC_Value = (high << 8) | low;

	return ADC_Value;
}
#endif /* ADC_H_ */