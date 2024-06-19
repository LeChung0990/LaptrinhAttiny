/*
 * _74HC595.h
 *
 * Created: 7/15/2023 2:42:59 PM
 *  Author: admin
 */ 
#include <avr/io.h>
#include <util/delay.h>

#define SH_CP       6   		//Pin 7 Attiny24A
#define DATA        5   		//Pin 8 Attiny24A
#define ST_CP       4   		//Pin 9 Attiny24A
#define	HC595_DDR	DDRA	
#define	HC595_PORT	PORTA	
#define HC595_DELAY _delay_us(50)
/* LED CODE 7 SEGMENT CATHODE COMMON IF WANT TO USING DOT: 0X80 IS DOT, 0X00 IS OFF DOT*/
const uint8_t SEG_MAP[] = {0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};
const uint8_t SEG_SELECT[] = {0XF7, 0XFB, 0XFD, 0XFE};

void HC595_Init();
void ONE7SEG(uint8_t Value);
void FOUR7SEG(unsigned char Digit, unsigned char Value);
void HIENTHI(int so);

void HC595_Init()
{
  HC595_DDR |= (1 << SH_CP) | (1 << DATA) | (1 << ST_CP);
}

void shcp()
{
  HC595_PORT |= (1 << SH_CP);   //Chan SH_CP = 1
  HC595_DELAY;
  HC595_PORT &= ~(1 << SH_CP); 	//Chan SH_CP = 0
  HC595_DELAY;
}

void strobe()
{
  /*Khi ST_CP = 1 se xuat du lieu cua thanh ghi SH_CP ra cac chan*/
  HC595_PORT |= (1 << ST_CP);
  HC595_DELAY;
  HC595_PORT &= ~(1 << ST_CP);
}

/*   Ap dung khi ket noi cac chan Q0->Q7 toi chan DP->A cua led 7 doan  */
void data(unsigned char data)
{
	for (unsigned char i = 0 ; i < 8; i++)
	{
		char t;
		t = (data >> i) & 0x01;     //Dich du lieu de lay tung bit trong 8 bit
		if (t == 1) {               //Xuat bit 0 hoac 1 ra chan Data
			HC595_PORT = HC595_PORT | (1 << DATA);
		}
		else {
			HC595_PORT = HC595_PORT & (~(1 << DATA));
		}
		shcp();                     //CHO 1 XUNG CHAN SH_CP DE XUAT 1 BIT RA CHAN DS
	}
}
void ONE7SEG(uint8_t Value)
{
  data(Value);
  strobe();
}

void FOUR7SEG(unsigned char Digit, unsigned char Value) {
  data(Value);
  data(Digit);		//Dich data so sau do moi dich ma Digit
  strobe();
}
void HIENTHI(int so)
{
  FOUR7SEG(SEG_SELECT[0], SEG_MAP[so / 1000]);
  FOUR7SEG(SEG_SELECT[1], SEG_MAP[(so / 100) % 10]);
  FOUR7SEG(SEG_SELECT[2], SEG_MAP[(so / 10) % 10]);
  FOUR7SEG(SEG_SELECT[3], SEG_MAP[(so / 1) % 10]);
}
