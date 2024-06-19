/*F_CPU = 8Mhz*/
void T0delay_us(unsigned int us)
{
	TCCR0A = 0;
	TCCR0B = 0;
	if(us<=256)
	{
		//F_CPU/8=1MHZ---> 1 tick = 1us
		TCCR0B |=1<<CS01;
		TCNT0 = 256-us;
		while((TIFR0 & 0x01) == 0);
		TIFR0 = 0x1;
	}
	else if(us<=2048)
	{
		//F_CPU/64 = 0.125MHZ---> 1 tick = 8us
		TCCR0B |=(1<<CS01)|(1<<CS00);
		TCNT0 = 256-(us/8);
		while((TIFR0 & 0x01) == 0);
		TIFR0 = 0x1;
	}
	else if(us<=8192)
	{
		//F_CPU/256 = 31250HZ---> 1 tick = 32us
		TCCR0B |=1<<CS02;
		TCNT0 = 256-(us/32);
		while((TIFR0 & 0x01) == 0);
		TIFR0 = 0x1;
	}
	else if(us<=32768)
	{
		//F_CPU/1024 = 7812.5HZ---> 1 tick = 128us
		TCCR0B |=(1<<CS02)|(1<<CS00);
		TCNT0 = 256-(us/128);
		while((TIFR0 & 0x01) == 0);
		TIFR0 = 0x1;
	}
}
