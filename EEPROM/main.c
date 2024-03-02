/*
 * EEPROM.c
 *
 * Created: 7/19/2023 9:51:48 AM
 * Author : admin
 */ 
 #define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include "74HC595.h"
void EEPROM_write(uint16_t Address, uint8_t Data);
uint8_t EEPROM_read(uint16_t Address);
uint8_t a=0;
int main(void)
{
	HC595_Init();
	EEPROM_write(127,100);
    /* Replace with your application code */
    while (1) 
    {
		a = EEPROM_read(127);
		HIENTHI(a);
    } 
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