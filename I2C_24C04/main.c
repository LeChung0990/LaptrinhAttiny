/*
 * 24C04_Attiny24A.c
 *
 * Created: 10/4/2023 12:35:48 PM
 * Author : admin
 */ 
#define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "I2C.h"

volatile uint8_t btn = 0;

void WriteByte_Eeprom24C04(unsigned char StartAddress, unsigned char Data)
{
	i2c_start();
	i2c_write(0xA0);  /* device write address 24c04 */
	i2c_write(StartAddress);

	i2c_write(Data);	/* Sequential Write*/
	i2c_stop();
}
unsigned char ReadByte_Eeprom24C04(unsigned char StartAddress)
{
	uint8_t result;
	i2c_start();
	i2c_write(0xA0);
	i2c_write(StartAddress);

	i2c_start();
	i2c_write(0xA1);
	result = i2c_read(0);		/*Nack if read Random read*/
	i2c_stop();
	return result;
}
int main(void)
{
    _delay_ms(10);
    WriteByte_Eeprom24C04(0x00, 0x34);
	_delay_ms(10);
	WriteByte_Eeprom24C04(0x10, 0x22);
	_delay_ms(10);
	WriteByte_Eeprom24C04(0x20, 0x23);
	_delay_ms(10);
	WriteByte_Eeprom24C04(0x30, 0x24);
	_delay_ms(10);
	btn = ReadByte_Eeprom24C04(0x30);
    while (1) {
		
    }
}

