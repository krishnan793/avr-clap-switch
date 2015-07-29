/*
 * RoomControl.c
 *
 * Created: 26-07-2015 04:01:21
 *  Author: ANANTHAKRISHNAN U S
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define F_CPU 1000000UL

#define RELAY_PORT PORTC
#define RELAY_CNTRL DDRC

int relay= -1;
void timer_init();

int main(void)
{
	timer_init();
	/* Enable output ports */
	RELAY_CNTRL |= 0xFF;					// First 4 bits are outputs
	RELAY_PORT = 0xFF;
	
	/* ENable INT0 (PIN 4) as interrupt*/
	GICR |= (1<<INT0);
	MCUCR |=(1<<ISC00);	// rising edge will trigger INT0
	sei();
	
    while(1)
    {

    }
}

ISR(INT0_vect)
{
	if (relay==-1)
	{
		relay = 1;
		timer_init();
	} else 
	{
		relay=relay+1;	
	}
	_delay_ms(100);
	GIFR |= (1<<INTF1);
}

void timer_init()
{
	TCNT1 = 0;
	TCCR1B |= (1<<WGM12)|(1<<CS11)|(1<<CS10);		// 64 pre scaling & CTC
	OCR1A = 19625;                              // ~1 sec
	TIMSK |= (1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{

	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS10);		// 64 pre scaling & CTC
	TIMSK &= ~(1<<OCIE1A);

	if (relay== 2)          // 2 clap
	{
		RELAY_PORT ^= 0x04;
	} 
	else if(relay == 3)     // 3 clap
	{
		RELAY_PORT ^= 0x08;
	}

	relay = -1;
	TCNT1 = 0;
}
