/*
 * I2C DS1307_update.c
 *
 * Created: 8/1/2023 9:30:09 PM
 * Author : admin
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <stdbool.h>
#include "I2Cpin.h"
#include "74HC595.h"

#define AddressWrite	0xD0
#define AddressRead		0xD1

#define buzzer			2	//B2, Pin 5
#define buttonSet		1	//B1, Pin 3
#define buttonUp		0	//B0, Pin 2
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;
uint8_t set = 0;
uint8_t baothucgio = 0;
uint8_t baothucphut = 10;
void NgatPCINT();
char decimal_to_bcd(uint8_t value);
char ReadData(uint8_t AddressData);
void WriteData(uint8_t AddressData, uint8_t Data);
void Buzzer();
void EEPROM_write(uint8_t Address, uint8_t Data);
uint8_t EEPROM_read(uint8_t Address);

int main(void)
{
	/*Khoi tao LED 7 doan va ngat tai chan INT0*/
	HC595_Init();
	NgatPCINT();
	/*Dat input cho nut nhan Up, output cho Buzzer*/
	DDRB &= ~(1<<buttonUp);
	PORTB |= (1<<buttonUp);	
	DDRB |= (1<<buzzer);
	TCCR0A &= ~(1<<COM0A1);
    while (1) 
    {
		/*Lay du lieu gio, phut, giay tu DS1307*/
		hour = ReadData(0x02);
		minute = ReadData(0x01);
		second = ReadData(0x00); 
		/*Nhap nhay dau cham de bieu thi dong ho dang chay*/
		if (second % 2 == 0) 
			{FOUR7SEG(SEG_SELECT[1], 0x80);}
		else 
			{FOUR7SEG(SEG_SELECT[1],0x00);}
		/*Hien thi gio, phut len LED 7 doan*/
		FOUR7SEG(SEG_SELECT[0], SEG_MAP[hour / 10]);
		FOUR7SEG(SEG_SELECT[1], SEG_MAP[hour % 10]);
		FOUR7SEG(SEG_SELECT[2], SEG_MAP[minute / 10]);
		FOUR7SEG(SEG_SELECT[3], SEG_MAP[minute % 10]);
		/*Neu nhan buttonUp thi reset gio, phut, giay ve 0
		gio o dinh dang 12h AM*/
		if(!(PINB & (1<<buttonUp)))
		{
			WriteData(0x00,0);
			WriteData(0x01,0);
			WriteData(0x02,0);	/*FORMAT 24 HOUR*/
		}
		if (hour == baothucgio)
		{
			if (minute == baothucphut)
			{
				Buzzer();
				baothucphut--;
				baothucgio--;
			}
		}
		/*-----------CHE DO XEM GIAY------------*/
		while (set == 1)
		{
			second = ReadData(0x00);
			FOUR7SEG(SEG_SELECT[2], SEG_MAP[second / 10]);
			FOUR7SEG(SEG_SELECT[3], SEG_MAP[second % 10]);
			/*Nhan nut buttonUp de thoat che do xem giay*/
			if (!(PINB & (1<< buttonUp)))	/*Chong doi phim*/
			{
				_delay_ms(100);
				while ((PINB & (1 << buttonUp)) == 0){}
				set = 0;
			}
		}
		/*---------CAI DAT GIO------------------*/
		while (set == 2)
		{
			if ((PINB & (1<< buttonUp)) == 0)	/*Chong doi phim*/
			{
				_delay_ms(100);
				while ((PINB & (1 << buttonUp)) == 0){}
				hour++;
				if (hour > 23) hour = 0;
			}
			WriteData(0x02,hour);
			FOUR7SEG(SEG_SELECT[0], SEG_MAP[hour / 10]);
			FOUR7SEG(SEG_SELECT[1], SEG_MAP[hour % 10]);
		}
		/*-------------CAI DAT PHUT--------------*/
		while (set == 3)
		{
			if ((PINB & (1<< buttonUp)) == 0)	/*Chong doi phim*/
			{
				_delay_ms(100);
				while ((PINB & (1 << buttonUp)) == 0){}
				minute++;
				if (minute > 59) minute = 0;
			}
			WriteData(0x01,minute);
			FOUR7SEG(SEG_SELECT[2], SEG_MAP[minute / 10]);
			FOUR7SEG(SEG_SELECT[3], SEG_MAP[minute % 10]);
		}
		/*-------------BAO THUC GIO--------------*/
		while (set == 4)
		{
			if ((PINB & (1<< buttonUp)) == 0)	/*Chong doi phim*/
			{
				_delay_ms(100);
				while ((PINB & (1 << buttonUp)) == 0){}
				baothucgio++;
				if (baothucgio > 23) baothucgio = 0;
				EEPROM_write(0,baothucgio);
			}
			baothucgio = EEPROM_read(0);
			FOUR7SEG(SEG_SELECT[0], SEG_MAP[baothucgio/10]);
			FOUR7SEG(SEG_SELECT[1], SEG_MAP[(baothucgio%10)]|0x80);
		}
		/*-------------BAO THUC PHUT--------------*/
		while (set == 5)
		{
			if ((PINB & (1<< buttonUp)) == 0)	/*Chong doi phim*/
			{
				_delay_ms(100);
				while ((PINB & (1 << buttonUp)) == 0){}
				baothucphut++;
				if (baothucphut > 59) baothucphut = 0;
				EEPROM_write(1,baothucphut);
			}
			baothucphut = EEPROM_read(1);
			FOUR7SEG(SEG_SELECT[1], 0x80);
			FOUR7SEG(SEG_SELECT[2], SEG_MAP[baothucphut/10]);
			FOUR7SEG(SEG_SELECT[3], SEG_MAP[baothucphut%10]);
		}
    }
}
void NgatPCINT()
{ /*B1 IS EXTERNAL INTERRUPTS, IS BUTTON SET, MODE PULLUP*/
	DDRB |= ~(1 << buttonSet);
	PORTB |= (1 << buttonSet);
	
	/*SET INT0 AS FALLING EDGE TRIGGER*/
	MCUCR |= (1 << ISC01);
	/*INTERRUPTS PIN CHANGE B1*/
	PCMSK1 |= (1<<PCINT9);
	GIMSK |= (1<<PCIE1);
	GIMSK |= (1 << INT0);
	sei();   /* SREG |= (1 << 7) */
}

ISR(PCINT1_vect)
{
	/* TAT LED 7 DOAN*/
	FOUR7SEG(SEG_SELECT[0], 0x00);
	FOUR7SEG(SEG_SELECT[1], 0x00);
	FOUR7SEG(SEG_SELECT[2], 0x00);
	FOUR7SEG(SEG_SELECT[3], 0x00);
	
	if ((PINB & (1<< buttonSet)) == 0)	/*Chong doi phim*/
	{
		_delay_ms(100);
		while ((PINB & (1 << buttonSet)) == 0){}
		set++;		
		if (set > 5) {set = 0;}
	}
}

char decimal_to_bcd(uint8_t value)
{
	uint8_t hex;
	hex = (((value / 10) << 4) + (value % 10));
	return hex;
}
char ReadData(uint8_t AddressData)
{
	uint8_t ResultData = 0;
	uint8_t temp;
	i2c_start();
	i2c_write(AddressWrite);
	i2c_write(AddressData);
	i2c_stop();

	i2c_start();
	i2c_write(AddressRead);
	ResultData =i2c_read();	
	Nack();
	i2c_stop();
	/*BCD to Decimal*/
	temp = (ResultData >> 4) & 0x0F;
	ResultData = ResultData & 0x0F;
	return ((temp*10)+ResultData);
}
void WriteData(uint8_t AddressData, uint8_t Data)
{
	i2c_start();
	i2c_write(AddressWrite);
	i2c_write(AddressData);
	i2c_write(decimal_to_bcd(Data));
	i2c_stop();
}
void Buzzer()
{
	PORTB |= (1<<buzzer);
	_delay_ms(1000);
	PORTB &= ~(1<<buzzer);
}
void EEPROM_write(uint8_t Address, uint8_t Data)
{
	while(EECR & (1<<EEPE));
	EECR = (0<<EEPM1)|(0<<EEPM0);
	EEAR = Address;
	EEDR = Data;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}
uint8_t EEPROM_read(uint8_t Address)
{
	while(EECR & (1<<EEPE));
	EEAR = Address;
	EECR |= (1<<EERE);
	return EEDR;
}