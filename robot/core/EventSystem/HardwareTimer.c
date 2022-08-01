#include "HardwareTimer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#define MICROS_RESOLUTION 100
#define PRESCALER 8
static volatile uint64_t microseconds = 0;

void timer_init() {
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | (_BV(CS11));
  TIMSK1 = (1 << (1));
  OCR1A = ((F_CPU / PRESCALER) / 10000) - 1;
  sei();
}

uint64_t millis() { return (micros() / 1000); }

uint64_t micros() {
  uint64_t us;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { us = microseconds; }

  return us;
}

ISR(TIMER1_COMPA_vect) { microseconds += MICROS_RESOLUTION; }