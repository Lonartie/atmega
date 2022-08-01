#include "HardwareTimer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <util/atomic.h>

#define MICROS_RESOLUTION 100
#define PRESCALER 8

static volatile uint64_t microseconds;

void timer_init() {
  // initialize timer 1 for microseconds counting
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | _BV(CS11);
  TIMSK1 = _BV(OCIE1A);
  OCR1A = ((F_CPU / PRESCALER) / 10000) - 1;
  sei();
}

uint64_t millis() { return (micros() / 1000); }

uint64_t micros() {
  uint64_t us;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { us = microseconds; }

  return us;
}
