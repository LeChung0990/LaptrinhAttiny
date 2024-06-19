/* PAGE 68 DOCUMENT ATTINY24A_ATMEL DATASHEET 

TIMER0 USING INTERRUPT FUNCTION 
--------------------------------
*/
#define __AVR_ATtiny24A__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
	DDRB |=(1<<1);
  cli();  // disable interrupt global
  /* Reset Timer/Counter0 */
  TCCR0A = 0;
  TCCR0B = 0;
  TIMSK0 = 0;
  /* Setup Timer/Counter0 */
  TCCR0B = 0x02;        	/*prescale/8, F_CPU = 8Mhz, -> 1 tick = 1us*/
  TCNT0 = 156;          	//interrupt at 100us -> 100 tick
  TIMSK0 |= (1 << TOIE0); 	// Overflow interrupt enable
  sei();                	// enable interrupt global
  while(1){
  }
}
/*Ham ngat Timer0*/
ISR (TIMER0_OVF_vect)
{
  TCNT0 = 156;
  PORTB ^= (1<<1);
}
