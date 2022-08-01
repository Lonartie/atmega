#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "Misc/Utils.h"

typedef enum WatchDogTime {
  NOW,
  MS_500,
  SEC_1,
  SEC_2,
  SEC_4,
  SEC_8
} WatchDogTime;

void watchdog_init(WatchDogTime time);
void stop_watchdog();

void reset_watchdog();

#endif  // WATCHDOG_H