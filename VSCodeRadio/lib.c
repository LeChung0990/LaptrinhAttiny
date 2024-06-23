#include "lib.h"
void i2c_init(void)
{
	SCL_1;
	SDA_1;
}
void i2c_start(void)
{
	/* SDA GOES LOW FIRST, SCL GOES LOW SECOND*/
	SCL_1;	I2C_DELAY;
	SDA_1;	I2C_DELAY;
	SDA_0;	I2C_DELAY;
	SCL_0;	I2C_DELAY;
}
void i2c_stop(void) {
	/* DAU TIEN CHO SCL XUONG THAP, CHUAN BI CHO SDA TU 
	THAP LEN CAO, SCL LEN MUC CAO TRUOC, SDA LEN MUC CAO THU HAI*/
	SDA_0;	I2C_DELAY;  
	SCL_1;	I2C_DELAY;
	SDA_1;	I2C_DELAY;
}

/*------------------Data Write and Data Read-------------*/
uint8_t i2c_write(uint8_t data)
{
	uint8_t result;
	/* TRANSMIT 8 BITS, MSB FIRST. IF BIT IS 1, SDA HIGH AFTER SCL HIGH
	*  SLAVE READS THE THIS BIT THEN SHIFT LEFT DATA BYTE  */
	uint8_t i;
	for( i = 0; i < 8; ++i)
	{
		if (data & 0x80) {	SDA_1;	}
		else {	SDA_0;	}
		I2C_DELAY;	//748n
		SCL_1;
		I2C_DELAY;
		SCL_0;
		data <<= 1;	//1.37u	
	}
	/*Kiem tra xung thu 9*/
	SDA_0;
	I2C_DELAY;

	SCL_1;
	I2C_DELAY;
	SCL_0;
	I2C_DELAY;

	SDA_1;
	if(SDA_VAL){
		result = 0;
	} else {
		result = 1;
	}
	I2C_DELAY;
	return result;
}

uint8_t i2c_read(uint8_t Ack)
{
	uint8_t u8DataRead = 0;
	uint8_t i;
	SDA_1;	
	for (i = 0; i < 8; ++i)
	{
		u8DataRead = u8DataRead << 1;
		SCL_1;
		I2C_DELAY;
		if (SDA_VAL)
		{
			u8DataRead = u8DataRead | 0x01;
		}
		I2C_DELAY;
		SCL_0;
		I2C_DELAY;
	}
  
	if(Ack){
		SDA_0;
	} else {
		SDA_1;
	}
	I2C_DELAY;

	SCL_1;
	I2C_DELAY;
	SCL_0;
	I2C_DELAY;
	return u8DataRead;
}
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
    ADCSRA |= (1 << ADPS2)|(1<<ADPS0);    // ADC PRESCALER /32
	ADCSRA &= ~(1 << ADEN);                  //ADC Disable
}
uint16_t ADC_Read(void)
{
	uint8_t low, high;
	uint16_t ADC_Value;
	ADCSRA |= (1 << ADSC);                //Start Conversion
	while ((ADCSRA & (1 << ADSC)) == 1);  // Wait conversion is complete
	low = ADCL;
	high = ADCH;
	ADC_Value = (high << 8) | low;
	return ADC_Value;
}
void EEPROM_write(uint16_t Address, uint8_t Data)
{
	while(EECR & (1<<EEPE));
	EECR = (0<<EEPM1)|(0<<EEPM0);
	EEAR = Address;
	EEDR = Data;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}
uint8_t EEPROM_read(uint16_t Address)
{
	while(EECR & (1<<EEPE));
	EEAR = Address;
	EECR |= (1<<EERE);
	return EEDR;
}