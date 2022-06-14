#ifndef SENSOR_WATCHER_H
#define SENSOR_WATCHER_H

#include "../Misc/utils.h"
#include "../Misc/String.h"
#include "../Models/Pin.h"

typedef struct SensorWatcher
{
  Pin pin;
  String event;
  bool last_state;

  void(*start)();
  void(*stop)();
} SensorWatcher;

SensorWatcher SensorWatcher_create(Pin pin, String event);
void SensorWatcher_start(SensorWatcher* _this);
void SensorWatcher_stop(SensorWatcher* _this);
void SensorWatcher_update(void* _this);

#endif // SENSOR_WATCHER_H