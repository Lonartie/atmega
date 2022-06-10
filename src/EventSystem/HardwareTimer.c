#include "HardwareTimer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint64_t timerMillis;

void initTimer()
{
  TCCR1B = (1 << WGM12);       // from the datasheet
  OCR1A = F_CPU / 1024 / 1000; // number of ticks in a second
  
  TIMSK1 = (1 << OCIE1A);      // from the data sheet
  TCCR1B |= (1 << CS12) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect) 
{
  timerMillis++;
}

uint32_t millis()
{
	uint32_t m;
	uint8_t oldSREG = SREG;
	cli();
	m = timerMillis;
	SREG = oldSREG;
	return m;
}