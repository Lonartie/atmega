#ifndef ANY_SENSOR_WATCHER_H
#define ANY_SENSOR_WATCHER_H

#include "../Misc/utils.h"
#include "../Misc/String.h"
#include "../Misc/Vector.h"
#include "../Models/Pin.h"

/// @brief A sensor watcher that takes up to 8 pins, watches their states
///        and if any state has changed it sends the specified event
typedef struct AnySensorWatcher
{
  Vector_Pin_8 pins;
  Vector__Bool_8 states;
  String event;
} AnySensorWatcher;

/// @brief creates a new sensor watcher with the specified event and pins (up to 8)
/// @returns a new sensor watcher which has not started yet
AnySensorWatcher AnySensorWatcher_create(String event, uint8_t n, ...);

/// @brief starts the given watcher
void AnySensorWatcher_start(AnySensorWatcher* _this);

/// @brief stops the given watcher
void AnySensorWatcher_stop(AnySensorWatcher* _this);

/// @brief updates the given watcher
void AnySensorWatcher_update(void* _this);

#endif // ANY_SENSOR_WATCHER_H