#ifndef UPDATER_H
#define UPDATER_H

#include "../Misc/Vector.h"

/// @brief the updater polls for events and sends them to the event system
typedef struct Updater {
  void* object;
  void (*update)(void*);
} Updater;

DECLARE_VECTORS(Updater);

/// @brief creates a new updater with the specified update function
/// @param object data of the update function
/// @param update the update function that will be called every time period
/// @returns a new updater with the specified update function
Updater Updater_create(void* object, void (*update)(void*));

#endif  // UPDATER_H