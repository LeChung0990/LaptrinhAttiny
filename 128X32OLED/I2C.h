/* Code for Attiny24A using interface I2C 
* Set input output of I/O PORT 
* Attiny24A using clock internal 8MHz */
#include <avr/io.h>
#include <util/delay.h>

#define I2C_SCL   1 	//Pin SCL PIN 13 (A1)
#define I2C_SDA   0 	//Pin SDA PIN 12 (A0)
#define I2C_DDR	  DDRA
#define I2C_PIN   PINA
#define I2C_DELAY _delay_us(10); 


/*SET INPUT WILL PULLED HIGH BY RESISTOR  -> LOGIC 1
  SET OUTPUT WILL PULLED LOW BY MCU       -> LOGIC 0  */
#define SCL_1 	I2C_DDR &= ~(1 << I2C_SCL);
#define SCL_0  	I2C_DDR |= (1 << I2C_SCL);
#define SDA_1 	I2C_DDR &= ~(1 << I2C_SDA);
#define SDA_0  	I2C_DDR |= (1 << I2C_SDA);
#define SDA_VAL (I2C_PIN & (1 << I2C_SDA))	

static void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t Ack);

static void i2c_init(void)
{
	SCL_1;
	SDA_1;
}

void i2c_start(void)
{
  /* SDA GOES LOW FIRST, SCL GOES LOW SECOND*/
  SCL_1;  I2C_DELAY;
  SDA_1;  I2C_DELAY;
  SDA_0;  I2C_DELAY;
  SCL_0;  I2C_DELAY;
}
void i2c_stop(void) {
  /* DAU TIEN CHO SCL XUONG THAP, CHUAN BI CHO SDA TU 
	THAP LEN CAO, SCL LEN MUC CAO TRUOC, SDA LEN MUC CAO THU HAI*/
  SDA_0;  I2C_DELAY;  
  SCL_1;  I2C_DELAY;
  SDA_1;  I2C_DELAY;
}

/*------------------Data Write and Data Read-------------*/
uint8_t i2c_write(uint8_t data)
{
  uint8_t i;
  uint8_t result;
  /* TRANSMIT 8 BITS, MSB FIRST. IF BIT IS 1, SDA HIGH AFTER SCL HIGH
   *  SLAVE READS THE THIS BIT THEN SHIFT LEFT DATA BYTE  */
  for (i = 0; i < 8; ++i)
  {
    if (data & 0x80) {SDA_1;}
    else {SDA_0;}
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
  
  SDA_1;
  
  for (uint8_t i = 0; i < 8; ++i)
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
