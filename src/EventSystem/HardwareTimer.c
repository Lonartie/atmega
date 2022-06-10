#include "HardwareTimer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile uint64_t timerMillis;

void initTimer()
{
	TCCR0A |= (1<<WGM01); // set CTC mode
	TCCR0B |= ((1<<CS01) | (1<<CS00)); //set counting clock to F_CPU / 8 -> 1MHZ
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 125; // set counter compare val,we count from 0-99 w 1MHZ CLK
}

ISR(TIMER1_COMPA_vect) 
{
  timerMillis++;
}

uint32_t millis()
{
  unsigned long  ms;	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		ms = timerMillis;
	}	
	return ms;
}