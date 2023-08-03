/* Code for Attiny24A using interface I2C 
* Set input output of I/O PORT 
* Attiny24A using clock internal 8MHz */
#include <avr/io.h>
#include <util/delay.h>

#define I2C_SCL   7 	//Pin SCL PIN 6 (A7)
#define I2C_SDA   3 	//Pin SDA PIN 10(A3)
#define I2C_DDR	  DDRA  
#define I2C_PIN   PINA  

/*DELAY DE TAO TAN SO GIAO TIEP F = 1/DELAY KHZ */
#define I2C_DELAY() _delay_us(10); 

/*SET INPUT WILL PULLED HIGH BY RESISTOR  -> LOGIC 1
  SET OUTPUT WILL PULLED LOW BY MCU       -> LOGIC 0  */
#define ONE_SCL() 	I2C_DDR &= ~(1 << I2C_SCL);	
#define NULL_SCL()  I2C_DDR |= (1 << I2C_SCL);	
#define ONE_SDA() 	I2C_DDR &= ~(1 << I2C_SDA); 
#define NULL_SDA()  I2C_DDR |= (1 << I2C_SDA); 	

void i2c_start(void);
void i2c_stop(void);
void ack(void);
void Nack(void);
void i2c_write(uint8_t data);
char i2c_read(void);
/*----------------- START AND STOP------------------*/
void i2c_start(void)
{
  /* SDA GOES LOW FIRST, SCL GOES LOW SECOND*/
  NULL_SDA();
  I2C_DELAY();
  NULL_SCL();
}
void i2c_stop(void) {
  /* DAU TIEN CHO SCL XUONG THAP, CHUAN BI CHO SDA TU 
	THAP LEN CAO, SCL LEN MUC CAO TRUOC, SDA LEN MUC CAO THU HAI*/
  NULL_SDA();
  ONE_SCL();
  I2C_DELAY();
  ONE_SDA();
}
/*----------------- Check ack and Nack------------------*/
void ack(void)
{
	/* THA SDA INPUT DE CHO SLAVE PHAT BIT ACK,DO LA CHU KY XUNG THU 9 CHO SCL LEN MUC CAO 
	* BIT ACK NAY BI BO QUA NEN CAN CO DELAY, SCL MUCNTHAP 1 LAN NUA  */
  ONE_SDA();
  ONE_SCL();
  I2C_DELAY();
  NULL_SCL();
}
void Nack(void)
{
  ONE_SDA();
  I2C_DELAY();
  ONE_SCL();
  I2C_DELAY();
  NULL_SCL();
  I2C_DELAY();
  ONE_SDA();
}
/*------------------Data Write and Data Read-------------*/
void i2c_write(uint8_t data)
{
  /* TRANSMIT 8 BITS, MSB FIRST. IF BIT IS 1, SDA HIGH AFTER SCL HIGH
   *  SLAVE READS THE THIS BIT THEN SHIFT LEFT DATA BYTE  */
  for (uint8_t i = 0; i < 8; i++)
  {
	  NULL_SDA();
	  if (data & 0x80) {ONE_SDA();}
	  I2C_DELAY();
	  ONE_SCL();
	  data <<= 1;
	  I2C_DELAY();
	  NULL_SCL();
  }
  ack();
}
char i2c_read(void)
{
  uint8_t u8DataRead = 0;
  ONE_SDA();
  for (uint8_t i = 0; i < 8; i++)
  {
    u8DataRead = u8DataRead << 1;
    ONE_SCL();
    I2C_DELAY();
    if ((I2C_PIN & (1 << I2C_SDA)))
	{
		 u8DataRead = u8DataRead | 0x01;
	}
    NULL_SCL();
    I2C_DELAY();
  }
  return u8DataRead;
}
