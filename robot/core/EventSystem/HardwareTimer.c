#include "HardwareTimer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#define MICROS_RESOLUTION 100
#define PRESCALER 8
static volatile uint64_t microseconds = 0;

void timer_init() {
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11);
  TIMSK1 = (1 << OCIE1A);
  OCR1A = ((F_CPU / PRESCALER) / 10000) - 1;
  sei();
}

uint64_t millis() {
  uint64_t us;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { us = microseconds; }

  return us / (1000 / MICROS_RESOLUTION);
}

uint64_t micros() {
  uint64_t us;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { us = microseconds; }

  return us * MICROS_RESOLUTION;
}

ISR(TIMER1_COMPA_vect) { microseconds += 1; }