#include "Reset.h"

#include <util/delay.h>

#include "Constants.h"
#include "Models/WatchDog.h"

void reset_system_5_seconds(System* atmega) {
  System_stop(atmega);
  _delay_ms(one_seconds_ms);
  watchdog_init(SEC_4);
  _delay_ms(five_seconds_ms);
  return;
}

void reset_system_now(System* atmega) {
  System_stop(atmega);
  watchdog_init(NOW);
  _delay_ms(one_seconds_ms);
  return;
}