#include "HardwareTimer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile uint64_t timerMillis;

void initTimer()
{
  TCCR1B = 0;
  TCCR1A = 0;


  //set CTC mode
  TCCR1B |= (1 << WGM12);
  // enable compare match interrupt
  TIMSK1 |= (1 << OCIE1A);


  // set OCR0A value for 100 msec
  OCR1A = 0x0619;
  //set 1024 prescaler
  TCCR1B |= (( 1 << CS10) | (1 << CS12));
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