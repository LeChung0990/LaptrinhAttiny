# Laptrinh Attiny24A

IDE Lập trình : Microchip Studio\
Mạch nạp : USB ISP 3.0\
Phần mềm nạp : Progisp 1.7

**Attiny24A là vi điều khiển thuộc họ AVR của hãng ATMEL với giá thành rẻ được trang bị 2KB bộ nhớ Flash 128byte EEPROM và 128byte SRAM, 4 kênh PWM, 2 timer 8 bit và 1 timer 16 bit, tần số dao động nội là 8MHz**

**Mua Attiny24A tại shop sau:**
<https://icdayroi.com/attiny24a-ssu>\
**Attiny24A có 14 chân và đóng gói dạng SOP**

# Code cơ bản về attiny24A

## 1. GPIO
a. Code

```c
#define F_CPU 8000000UL	//khai bao thach anh de su dung thu vien delay
#include <avr/io.h>     //thu vien io cho attiny
#include <util/delay.h> //thu vien delay
#define	led     6   //LED pin A6
#define button  7   //BUTTON pin A7
int main(void)
{
    DDRA &= ~(1<<button);   //button ngo vao
    DDRA |= (1<<led);       //led ngo ra
    PORTA |= (1<<button);   //BUTTON = INPUT PULLUP
    PORTB &= ~(1<<led);     // LED OFF
	while (1)
	{
        //CHECK BUTTON
        if((PINA & (1<< button))==0)
        {
            _delay_ms(100);
            if((PINA & (1<< button))==0)
            {
                PORTA ^= (1<<led);  // dung toan tu XOR de dao trang thai led
            }
        }
	}
}
```

## 2. Timer
1. **Timer0**
Các bước thiết lập Timer0
    1. SET VALUE FOR TCNT0 REGISTER
    2. SET MODE NORMAL OF TIMER0. SET PRESCALE
    3. CHECK FLAG TOV0, IF TOV0 == 1 THEN TIMER0 OVERFLOW
    4. TURN OFF TIMER0 BY WRITING 0 TO TCCR0 REGISTER
    5. CLEAN FLAG TOV0 BY WRITING 1 TO BIT TOV0
