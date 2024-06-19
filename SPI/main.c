/*
 * SPI_Attiny.c
 *
 * Created: 7/29/2023 11:10:35 AM
 * Author : admin
 */ 
#define __AVR_ATtiny24A__
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

/* SS or CS pin*/
#define SS_PIN 7
/*DO (Data out)*/
#define DO_PIN 5
/*DI (Data in)*/
#define DI_PIN 6
/* CLK */
#define CLK_PIN 4


void SPI_SetUp();
void setGPIO(uint8_t);
void setIODIR(uint8_t);
void WriteSPI(uint8_t);
char SlaveSPI();

volatile char temp;

int main(void)
{
	//uint8_t receive = 0;
	SPI_SetUp();
	setIODIR(0x00);		// make GPIO pins be outputs
	setGPIO(0b10101010); //Turn off all leds
	while(1)
	{
		
	}
}

void SPI_SetUp()
{
	DDRA |= (1<<DO_PIN)|(1<<SS_PIN)|(1<<CLK_PIN);
	DDRA &= ~(1<<DI_PIN);
	
	/*SPI config*/
	USICR = (1<<USIWM0);
	/* SS_PIN on, DI_PIN enable pull-up*/
	PORTA |= (1<<SS_PIN)|(1<<DI_PIN);
}

void setIODIR(uint8_t data)
{
	PORTA &= ~(1<<SS_PIN); 	//Select Slave
	WriteSPI(0x40);	// Slave Address 
	WriteSPI(0x00);	//IODIR address
	WriteSPI(data); //IODIR direction
	PORTA |= (1<<SS_PIN);	//Deselect Slave
}

void setGPIO(uint8_t data)
{
	PORTA &= ~(1<<SS_PIN);
	WriteSPI(0x40);	// Slave Address 
	WriteSPI(0x09);	// GPIO Address
	WriteSPI(data);	// GPIO Value, 0 or 1 , ON or OFF
	PORTA |= (1<<SS_PIN);
}

void WriteSPI(uint8_t data)
{
	//load USI Data register with data to transmit
	USIDR = data;
	//transmit the byte (8 bits)
	for(char i=0; i<8; i++)
	{
		USICR = (1<<USIWM0)|(1<<USITC);	//toggle clock pin
		USICR = (1<<USIWM0)|(1<<USICLK)|(1<<USITC); //toggle clock pin and register counter
	}
}
char SlaveSPI()
{	
	uint8_t data = 0;
	USICR |= (1<<USIWM0)|(1<<USICS1);
	if((USISR & (1<<USIOIF)))
	{
		data = USIBR;
		USISR |= (1<<USIOIF);	//Clearing counter overflow flag
	}	
	return data;
}
