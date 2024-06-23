/* Code for Attiny24A using interface I2C 
* Set input output of I/O PORT 
* Attiny24A using clock internal 8MHz */
#define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define I2C_SDA   1 	//Pin SCL PIN 12 (A1)
#define I2C_SCL   0 	//Pin SDA PIN 13 (A0)
#define I2C_DDR	  DDRA
#define I2C_PIN   PINA
#define I2C_DELAY _delay_us(5); 

/*SET INPUT WILL PULLED HIGH BY RESISTOR  -> LOGIC 1
  SET OUTPUT WILL PULLED LOW BY MCU       -> LOGIC 0  */
#define SCL_1 	I2C_DDR &= ~(1 << I2C_SCL)
#define SCL_0  	I2C_DDR |= (1 << I2C_SCL)
#define SDA_1 	I2C_DDR &= ~(1 << I2C_SDA)
#define SDA_0  	I2C_DDR |= (1 << I2C_SDA)
#define SDA_VAL (I2C_PIN & (1 << I2C_SDA))	


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t Ack);
void ADC_Init(uint8_t channel);
uint16_t ADC_Read(void);
void EEPROM_write(uint16_t Address, uint8_t Data);
uint8_t EEPROM_read(uint16_t Address);