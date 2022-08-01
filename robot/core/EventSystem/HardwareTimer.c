#include "HardwareTimer.h"

#include "libs/micros.h"

void timer_init() {  // Timer settings
  SET_TCCRA();
  SET_TCCRB();
  REG_TIMSK = _BV(BIT_OCIE);
  REG_OCR = ((F_CPU / PRESCALER) / 10000) -
            1;  // 10000 is the frequency (1 / 10000) * 1000000 = (100 uS)
  sei();
}

uint64_t millis() { return (micros() / 1000); }

uint64_t micros() {
  micros_t us;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { us = microseconds; }

  return us;
}

ISR(ISR_VECT) { microseconds += MICROS_RESOLUTION; }