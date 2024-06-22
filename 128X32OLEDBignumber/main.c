/*
 * OLEDBignumber.c
 *
 * Created: 7/31/2023 4:53:37 PM
 * Author : admin
 */ 
 #define __AVR_ATtiny24A__
#define  F_CPU 8000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "I2C.h"

// OLED definitions
//#define OLED_ADDR       0x78                  // OLED write address
#define OLED_ADDR       0x7A                  // Simulation
#define OLED_CMD_MODE   0x00                  // set command mode
#define OLED_DAT_MODE   0x40                  // set data mode
#define OLED_INIT_LEN   15                    // 15: no screen flip, 17: screen flip
// OLED init settings
const uint8_t OLED_INIT_CMD[] PROGMEM = {
	0xA8, 0x3F,       // set multiplex (HEIGHT-1): 0x1F for 128x32, 0x3F for 128x64
	0x22, 0x00, 0x03, // set min and max page
	0x20, 0x01,       // set vertical memory addressing mode
	0xDA, 0x02,       // set COM pins hardware configuration to sequential
	0x8D, 0x14,       // enable charge pump
	0xAF,             // switch on OLED
	0x00, 0x10, 0xB0, // set cursor at home position
	0xA1, 0xC8        // flip the screen
};

// simple reduced 3x8 font
const uint8_t OLED_FONT[] PROGMEM = {
	0x7F, 0x41, 0x7F, // 0  0
	0x00, 0x00, 0x7F, // 1  1
	0x79, 0x49, 0x4F, // 2  2
	0x41, 0x49, 0x7F, // 3  3
	0x0F, 0x08, 0x7E, // 4  4
	0x4F, 0x49, 0x79, // 5  5
	0x7F, 0x49, 0x79, // 6  6
	0x03, 0x01, 0x7F, // 7  7
	0x7F, 0x49, 0x7F, // 8  8
	0x4F, 0x49, 0x7F, // 9  9
	0x7F, 0x09, 0x7F, // A 10
	0x7F, 0x48, 0x78, // b 11
	0x7F, 0x41, 0x63, // C 12
	0x78, 0x48, 0x7F, // d 13
	0x7F, 0x49, 0x41, // E 14
	0x7F, 0x09, 0x01, // F 15
	0x00, 0x60, 0x00, // . 16
	0x00, 0x36, 0x00, // : 17
	0x08, 0x08, 0x08, // - 18
	0x00, 0x00, 0x00  //   19
};
// OLED init function
void OLED_init(void) {
	i2c_start();                             // initialize I2C first
	i2c_write(OLED_ADDR);                   // start transmission to OLED
	i2c_write(OLED_CMD_MODE);               // set command mode
	for (uint8_t i = 0; i < OLED_INIT_LEN; i++) i2c_write(pgm_read_byte(&OLED_INIT_CMD[i])); // send the command bytes
	i2c_stop();                             // stop transmission
}

void OLED_cursor(uint8_t xpos, uint8_t ypos) {
	i2c_start();
	i2c_write(OLED_ADDR);                   // start transmission to OLED
	i2c_write(OLED_CMD_MODE);               // set command mode
	i2c_write(xpos & 0x0F);                 // set low nibble of start column
	i2c_write(0x10 | (xpos >> 4));          // set high nibble of start column
	i2c_write(0xB0 | (ypos & 0x07));        // set start page
	i2c_stop();                             // stop transmission
}
void OLED_clear(void) {
	OLED_cursor(0, 0);                      // set cursor at upper left corner
	i2c_start();
	i2c_write(OLED_ADDR);                   // start transmission to OLED
	i2c_write(OLED_DAT_MODE);               // set data mode
	for(uint16_t i=512; i; i--) i2c_write(0x00); // clear the screen
	i2c_stop();                            // stop transmission
}
// OLED stretch a part of a byte
uint8_t OLED_stretch(uint8_t b) {
	b  = ((b & 2) << 3) | (b & 1);          // split 2 LSB into the nibbles
	b |= b << 1;                            // double the bits
	b |= b << 2;                            // double them again = 4 times
	return b;                               // return the value
}
// OLED print a big digit
void OLED_printD(uint8_t ch) {
	uint8_t i, j, k, b;                     // loop variables
	uint8_t sb[4];                          // stretched character bytes
	ch += ch << 1;                          // calculate position of character in font array
	for(i=8; i; i--) i2c_write(0x00);       // print spacing between characters
	for(i=3; i; i--) {                      // font has 3 bytes per character
		b = pgm_read_byte(&OLED_FONT[ch++]);  // read character byte
		for(j=0; j<4; j++, b >>= 2) sb[j] = OLED_stretch(b);  // stretch 4 times
		j=4; if(i==2) j=6;                    // calculate x-stretch value
		while(j--) {                       // write several times (x-direction)
			for(k=0; k<4; k++) i2c_write(sb[k]);// the 4 stretched bytes (y-direction)
		}
	}
}
// OLED print buffer
void OLED_printB(uint8_t *buffer) {
	i2c_start();
	i2c_write(OLED_ADDR);                   // start transmission to OLED
	i2c_write(OLED_DAT_MODE);               // set data mode
	for(uint8_t i=0; i<8; i++) OLED_printD(buffer[i]);  // print buffer
	i2c_stop();                             // stop transmission
}

int main(void)
{
	uint8_t buffer[8] = {0, 1, 2, 3, 4, 5, 6, 7};       // screen buffer

	OLED_init();                            // initialize the OLED
	OLED_clear();
    
	OLED_cursor(0,0);
	i2c_start();
	i2c_write(OLED_ADDR);                   // start transmission to OLED
	i2c_write(OLED_DAT_MODE);               // set data mode
	OLED_printD(1);  // print buffer
	OLED_printD(2);
	OLED_printD(17);
	OLED_printD(2);
	OLED_printD(0);
	OLED_printD(17);
	OLED_printD(5);
	OLED_printD(9);
	i2c_stop();                             // stop transmission
	
	OLED_printB(buffer);
    while (1) 
    {
    }
}

