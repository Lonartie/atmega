#include "HardwareTimer.h"

#include "libs/micros.h"

void timer_init() {
  REG_TCCRA = 0;
  REG_TCCRB = _BV(BIT_WGM) | CLOCKSEL;
  REG_TIMSK = (1 << (1));
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