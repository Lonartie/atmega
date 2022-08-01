#ifndef RESET_H
#define RESET_H

#include "Models/System.h"

void reset_system_5_seconds(System* atmega);
void reset_system_now(System* atmega);

#endif // RESET_H