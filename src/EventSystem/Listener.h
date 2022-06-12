#ifndef LISTENER_H
#define LISTENER_H

#include "String.h"
#include "Event.h"
#include "../Misc/utils.h"

typedef void(*ListenerCallback)();

typedef struct {
  ListenerCallback callback;
  String event_type;
} Listener;

DECLARE_VECTORS(Listener);

Listener Listener_create(ListenerCallback callback, String event_type);

#endif // LISTENER_H