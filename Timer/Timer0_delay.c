//#include "delay_Timer0.h"
//#include "avr/delay.h"
/* USING CRYSTAL 8MHZ */
//#define "avr/io.h"

/* 
1. SET VALUE FOR TCNT0 REGISTER
2. SET MODE NORMAL OF TIMER0. SET PRESCALE
3. CHECK FLAG TOV0, IF TOV0 == 1 THEN TIMER0 OVERFLOW
4. TURN OFF TIMER0 BY WRITING 0 TO TCCR0 REGISTER
5. CLEAN FLAG TOV0 BY WRITING 1 TO BIT TOV0 

*/

void T0delay(){
  TCNT0 = 156;		
  TCCR0B = 0x02;	/*PRESCALE/8, F_CPU=8MHZ, 1 TICK=1US, 100 US-100 TICK*/
  while((TIFR0 & 0x01) == 0); /*WHILE*/
  TCCR0A = 0;
  TCCR0B = 0;
  TIFR0 = 0x1;
}
void delay_()
{
  unsigned int t;
  for(t=0;t<10000;t++){
    T0delay();
  }
}
void setup() {
  // put your setup code here, to run once:
  DDRB |= (1 << PB0);
}

void loop() {
  // put your main code here, to run repeatedly:
  PORTB |= (1 << PB0);
  delay_();
  PORTB &= ~(1 << PB0);
  delay_();
}
