#define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lib.h"
// ===================================================================================
// RDA5807M Implementation
// ===================================================================================
#define ADDR_5807M	0x20	// Chip RDA5807M Address
#define buttonUp	0		//PB0, Pin 2
#define butFind		1		//PB1, pin 3
uint8_t channel = 129;			//set default channel
uint8_t flagSeek = 0;
uint8_t SeekComp = 0;
uint16_t ReadFre;
uint16_t ReadADC = 0;
uint8_t dem = 0;
/////////////////////////////////////////////////
void SetChannel(uint8_t kenh){
	uint8_t HighByte = (kenh >> 2);
	uint8_t LowByte = ((kenh & 0b11) << 6 ) | 0b00010000;
	
	i2c_start();
	i2c_write(ADDR_5807M);	
	i2c_write(0xC0);  //ghi bit cao vao thanh ghi 0x02
	i2c_write(0x01);  //ghi bit thap vao thanh ghi 0x02
	i2c_write(HighByte);
	i2c_write(LowByte);
	i2c_stop();
}
uint16_t ReadDataChip(uint8_t Register){
	i2c_start();
	i2c_write(0x22);
	i2c_write(Register);
	i2c_stop();	
	
	i2c_start();
	i2c_write(0x23);
	uint8_t HighByte = i2c_read(1);
	uint8_t LowByte = i2c_read(0);
	i2c_stop();
	return (HighByte<<8) | LowByte ;
}
// ===================================================================================
// Main Function
// ===================================================================================
int main(void)
{
	/*Radio*/
	DDRB &= ~(1<<buttonUp) & ~(1<<butFind);
	PORTB |= (1<<buttonUp) | (1<<butFind);
    DDRA |= (1<<6);
    PORTA &= ~(1<<6);

    cli();  // disable interrupt global
    /* Reset Timer/Counter0 */
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = 0;
    /* Setup Timer/Counter0 */
    TCCR0B = 0x05;        	/*prescale/1024, F_CPU = 8Mhz, -> 1 tick = 128us*/
    TCNT0 = 22;          	//interrupt at 30 000us -> 234 tick
    TIMSK0 |= (1 << TOIE0); 	// Overflow interrupt enable
    sei();                	// enable interrupt global

	/*display channel default is: channel mhz*/
    SetChannel(channel);
    ADC_Init(7); //PA7

    while (1) 
    {
        if(dem >= 0 && dem <=30){
            ADCSRA |= (1 << ADEN);                  
            ReadADC =  ADC_Read();
            if(ReadADC > 814)  PORTA |= (1<<6);
            else  PORTA &= ~(1<<6);
             
        }else {
            ADCSRA &= ~(1 << ADEN); 
            PORTA &= ~(1<<6);
        }
        
		//  if (!(PINB & (1<< buttonUp)))	
		// {
		// 	_delay_ms(200);
		// 	channel++;
		// 	while (!(PINB & (1 << buttonUp)))
		// 	{
		// 		_delay_ms(50);
		// 		channel++;
		// 	}
		// 	if(channel > 210) channel = 0;
		// 	SetChannel(channel);
		// }

		// if (!(PINB & (1<< butFind)))
		// {
		// 	_delay_ms(200);
		// 	while (!(PINB & (1 << butFind)));
		// 	i2c_start();  //seek up
		// 	i2c_write(ADDR_5807M);
		// 	i2c_write(0xC3);
		// 	i2c_write(0x01);
		// 	i2c_stop();
        //     while(!flagSeek){
        //         flagSeek = (uint8_t)(ReadDataChip(0x0A)>>14) & 0x1;
        //     }
		// }
    }
}
/*Ham ngat Timer0*/
ISR (TIM0_OVF_vect)
{
  TCNT0 = 22;
  dem ++;
  if(dem > 60) dem = 0;
}