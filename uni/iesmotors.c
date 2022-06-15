#include "iesmotors.h"

/**
  Sets up timer 0 (8-bit-timer)
*/
void setupTimer0() {
  // Disable all interrupts
  cli();
  // Set prescaler to 64, cf. datasheet for TCCR0B
  // (TCCR0B: Timer/Counter Control Register 0 B)
  TCCR0B = 0;
  TCCR0B |= (1 << CS00) | (1 << CS01);
  // Set waveform generation mode to Fast PWM, frequency = F_CPU / (PRESCALER * 2^8)
  TCCR0A = 0;
  TCCR0A |= (1 << WGM00) | (1 << WGM01);
  // Re-enable all interrupts
  sei();
}

/**
  Sets duty-cycle at pin PD5 or PD6 (OC0A or OC0B) to a value
  (0 - 255 = 0% - 100%).

  Timer0 needs to be setup before usage!

  The required pins also need to be set as output first. That
  setup is NOT done in the respective timer-setup-method!
*/
void setDutyCycle(uint8_t pin, uint8_t value)
{
  // Suggestion to handle PD6 - note the code-clones wrt. PD5 below!
  // Code-clones are extraordinary f cky! Correct this (tricky though
  // due to the PP-macros, which you cannot simply pass to functions)!
  // (But PP-macros can help you here....)
  if (pin == PD6) {
    if (value == 0) {
      TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);  // Normal port operation mode
      PORTD &= ~(1 << PD6);                       // PD6 LOW, equals 0% duty,
    }                                             // timer disconnected
    else if (value == 255) {
      TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);  // Normal port operation mode
      PORTD |= (1 << PD6);                        // PD6 HIGH, equals 100% duty,
    }                                             // timer disconnected
    else {
      TCCR0A |= (1 << COM0A1);                    // OC0A to LOW on Compare Match,
      TCCR0A &= ~(1 << COM0A0);                   // to HIGH at BOTTOM (non-inverting mode)
      OCR0A = value;                              // generates sequences of 1-0-1-0...
    }                                             // for certain periods of time
  }

  if (pin == PD5) {
    if (value == 0) {
      TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);
      PORTD &= ~(1 << PD5);
    }
    else if (value == 255) {
      TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);
      PORTD |= (1 << PD5);
    }
    else {
      TCCR0A |= (1 << COM0B1);
      TCCR0A &= ~(1 << COM0B0);
      OCR0B = value;
    }
  }
}
