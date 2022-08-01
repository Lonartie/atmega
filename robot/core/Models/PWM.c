#include "PWM.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "Misc/Utils.h"

void PWM_init() {
  DDRD = (1 << DD5) | (1 << DD6);  // set pins to output

  cli();                                  // disable interrupts
  TCCR0B = 0;                             // disable timer0
  TCCR0B |= (1 << CS00) | (1 << CS01);    // sets prescaler
  TCCR0A = 0;                             // set timer0 to normal mode
  TCCR0A |= (1 << WGM00) | (1 << WGM01);  // set fast PWM mode
  sei();                                  // enable interrupts
}

void PWM_set_duty_cycle(uint8_t pin, uint8_t value) {
  if (pin == PD6) {
    if (value == 0) {
      TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);
      PORTD &= ~(1 << PD6);
    } else if (value == 255) {
      TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);
      PORTD |= (1 << PD6);
    } else {
      TCCR0A |= (1 << COM0A1);
      TCCR0A &= ~(1 << COM0A0);
      OCR0A = value;
    }
  }

  if (pin == PD5) {
    if (value == 0) {
      TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);
      PORTD &= ~(1 << PD5);
    } else if (value == 255) {
      TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);
      PORTD |= (1 << PD5);
    } else {
      TCCR0A |= (1 << COM0B1);
      TCCR0A &= ~(1 << COM0B0);
      OCR0B = value;
    }
  }
}