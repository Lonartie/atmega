#ifndef SENSOR_WATCHER_H
#define SENSOR_WATCHER_H

#include "Misc/String.h"
#include "Models/Pin.h"

/// @brief watches a sensor (pin) and triggers an event when the sensor has
/// changed state
typedef struct SensorWatcher {
  Pin pin;
  String event;
  bool last_state;
} SensorWatcher;

/// @brief creates a new sensor watcher with the given pin and event
/// @param pin the pin to watch
/// @param event the event to trigger when the pin changes state
/// @returns a new sensor watcher
SensorWatcher SensorWatcher_create(Pin pin, String event);

/// @brief starts the watcher
void SensorWatcher_start(SensorWatcher* _this);

/// @brief stops the watcher
void SensorWatcher_stop(SensorWatcher* _this);

/// @brief updates the watcher
void SensorWatcher_update(void* _this);

#endif  // SENSOR_WATCHER_H