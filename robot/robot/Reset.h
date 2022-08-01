#ifndef RESET_H
#define RESET_H

#include "Models/System.h"

/// @brief resets the system in 5 seconds
void reset_system_5_seconds(System* atmega);

/// @brief resets the system right now (15ms but who cares)
void reset_system_now(System* atmega);

#endif  // RESET_H