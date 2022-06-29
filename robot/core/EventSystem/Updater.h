#ifndef UPDATER_H
#define UPDATER_H

#include "../Misc/Vector.h"

/// @brief the updater polls for events and sends them to the event system
typedef struct Updater {
  void* object;

  /// @brief an update function that the event system will call every time period
  /// @param object receiver of the update function
  void (*update)(void* object);
} Updater;

DECLARE_VECTORS(Updater);

/// @brief creates a new updater with the specified update function
/// @param object receiver of the update function
/// @param update the update function that will be called every time period
/// @returns a new updater with the specified update function
Updater Updater_create(void* object, void (*update)(void* object));

#endif // UPDATER_H