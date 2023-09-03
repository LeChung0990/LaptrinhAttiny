# Laptrinh Attiny24A

IDE Lập trình : Microchip Studio
Mạch nạp : USB ISP 3.0
Phần mềm nạp : Progisp 1.7

## 1.GPIO
a. Code

```c
#define F_CPU 8000000UL	//FREQUENCY 8MHZ
#include <avr/io.h>
#include <util/delay.h>
#define	led     6   //LED A6
#define button  7   //BUTTON A7
int main(void)
{
    DDRA &= ~(1<<button);
    DDRA |= (1<<led);
    //BUTTON = INPUT PULLUP
    PORTA |= (1<<button);
    // LED OFF
    PORTB &= ~(1<<led);
	while (1)
	{
        //CHECK BUTTON
        if((PINA & (1<< button))==0)
        {
            _delay_ms(100);
            if((PINA & (1<< button))==0)
            {
                PORTA ^= (1<<led);
            }
        }
	}
}
```
