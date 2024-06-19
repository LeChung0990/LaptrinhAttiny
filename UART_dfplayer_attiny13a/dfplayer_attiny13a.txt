//DFPlayer Mini ATtiny13A 20200724 0300

#include <avr/io.h>
#define F_CPU 9600000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

uint8_t i,k,ct,total,dat,res[10],rpt;
int8_t vol,num;
uint16_t va;

void eep_write(uint8_t ads,uint8_t dat){ //EEPROM Write

	while(EECR & (1<<EEPE));	//Wait
	EECR=(0<<EEPM1)|(0<<EEPM0);	//Programing Mode
	EEAR=ads;					//Address
	EEDR=dat;					//Data
	EECR|=(1<<EEMPE);			//Enable
	EECR|=(1<<EEPE);			//Start
}

void eep_read(uint8_t ads){ //EEPROM Read

	while(EECR & (1<<EEPE));	//Wait
	EEAR=ads;					//Address
	EECR|=(1<<EERE);			//Read
	dat=EEDR;					//Data
}

void tx(uint8_t com,uint8_t dat){ //UART TX

	uint8_t t[]={0x7E,0xFF,0x06,com,0x00,0x00,dat,0xEF};
	_delay_ms(20);
	for(k=0;k<8;k++){
		PORTB&=~(1<<2);_delay_us(104); //Start
		for(i=0;i<8;i++){
			if((t[k]>>i)&1){PORTB|=(1<<2);}else{PORTB&=~(1<<2);}
			_delay_us(98);
		}
		PORTB|=(1<<2);_delay_us(104); //Stop
	}
}

ISR(PCINT0_vect){ //UART RX

	for(k=0;k<10;k++){
		ct=0;
		while(PINB & 0x02){
			ct++;
			_delay_us(1);
			if(ct>250){break;}
		}
		_delay_us(156);
		for(i=0;i<8;i++){
			res[k]>>=1;
			if(PINB & 0x02){res[k]|=(1<<7);}
			_delay_us(98);
		}
	}

	if(res[3]==0x48){total=res[6];} //Total Number

	if(res[3]==0x3D){ //Next Music
		if(!rpt){num++;}
		if(num>total){num=1;}
		tx(0x03,num);
		eep_write(0x00,num);
	}
}

void music(){
	tx(0x03,num);
	eep_write(0x00,num);
	_delay_ms(50);
}

void volume(){
	tx(0x06,vol);
	eep_write(0x02,vol);
	_delay_ms(150);
}

int main(void){

	CLKPR =0b10000000; //Prescaler Change Enable
	CLKPR =0b00000000; //Div1(9.6MHz)

	DDRB  =0b00000100;
	PORTB =0b00011101; //PB4:Next ,PB3:Volume, PB2:TX, PB1:RX, PB0:Previous

	GIMSK =0b00100000; //PCINT Enable
	PCMSK =0b00000010; //PCINT1

	ADMUX =0b00000011; //Reference:VCC, ADC3 Enable
	ADCSRA=0b10000000; //ADC Eable, CK/2
	DIDR0 =0b00001000; //Digtal Input Disable(ADC3)
	sei();

	_delay_ms(1500);
	tx(0x48,0); //Query Total Number

	eep_read(0x02); //Read Volume
	vol=dat;
	if(vol>30 || vol<0){vol=10;}
	tx(0x06,vol); //Volume

	eep_read(0x00); //Read Music Number
	num=dat;
	if(num>total){num=1;}
	tx(0x03,num); //Play
	eep_write(0x00,num);

	//-------------------------------------
	while(1){

		ADCSRA|=0b01000000;
		while(ADCSRA & 0b01000000);
		va=ADC;

		if(va>490 && va<590){ //Volume Up
			vol++;
			if(vol>30){vol=30;}
			volume();
		}

		if(va>170 && va<270){ //Volume Down
			vol--;
			if(vol<0){vol=0;}
			volume();
		}

		if(va<100){ //Repeat
			rpt^=1;
			_delay_ms(1000);
		}

		if(~PINB & 0x01){ //PB0:Previous
			num--;
			if(num<1){num=total;}
			music();
			while(~PINB & 0x01){}
			_delay_ms(50);
		}

		if(~PINB & 0x10){ //PB4:Next
			num++;
			if(num>total){num=1;}
			music();
			while(~PINB & 0x10){}
			_delay_ms(50);
		}
	}
}