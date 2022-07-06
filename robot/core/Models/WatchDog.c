#include "WatchDog.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

void watchdog_init(WatchDogTime time) {
  cli();
  wdt_reset();            // Reset Watchdog Timer
  MCUSR &= ~(1 << WDRF);  // Rücksetzen des Watchdog System Reset Flag
  WDTCSR = (1 << WDCE) | (1 << WDE);  // Watchdog Change Enable

  switch (time) {
    case MS_500:
      WDTCSR = (1 << WDP0) | (1 << WDP2);
      break;
    case SEC_1:
      WDTCSR = (1 << WDP1) | (1 << WDP2);
      break;
    case SEC_2:
      WDTCSR = (1 << WDP0) | (1 << WDP1) | (1 << WDP2);
      break;
    case SEC_4:
      WDTCSR = (1 << WDP3);
      break;
    case SEC_8:
      WDTCSR = (1 << WDP0) | (1 << WDP3);
      break;
  }

  WDTCSR |= (1 << WDIE);  // Watchdog Interrupt enable
  sei();
}

void stop_watchdog() {
  WDTCSR &= ~(1 << WDIE);  // disable
}

void reset_watchdog() {
  MCUSR &= ~(1 << WDRF);  // Rücksetzen des Watchdog System Reset Flag
}
