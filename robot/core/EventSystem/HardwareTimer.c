#include "HardwareTimer.h"

#include "libs/micros.h"

void timer_init() {
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | (_BV(CS11));
  TIMSK1 = (1 << (1));
  OCR1A = ((F_CPU / 8) / 10000) - 1;
  sei();
}

uint64_t millis() { return (micros() / 1000); }

uint64_t micros() {
  micros_t us;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { us = microseconds; }

  return us;
}

ISR(ISR_VECT) { microseconds += MICROS_RESOLUTION; }