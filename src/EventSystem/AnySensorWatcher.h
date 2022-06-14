#ifndef ANY_SENSOR_WATCHER_H
#define ANY_SENSOR_WATCHER_H

#include "../Misc/utils.h"
#include "../Misc/String.h"
#include "../Misc/Vector.h"
#include "../Models/Pin.h"

typedef struct AnySensorWatcher
{
  Vector_Pin_8 pins;
  Vector__Bool_16 states;
  String event;

  void(*start)();
  void(*stop)();
} AnySensorWatcher;

AnySensorWatcher AnySensorWatcher_create(String event, uint8_t n, ...);
void AnySensorWatcher_start(AnySensorWatcher* _this);
void AnySensorWatcher_stop(AnySensorWatcher* _this);
void AnySensorWatcher_update(void* _this);

#endif // ANY_SENSOR_WATCHER_H