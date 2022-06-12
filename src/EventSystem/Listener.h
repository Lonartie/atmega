#ifndef LISTENER_H
#define LISTENER_H

#include "String.h"
#include "../Misc/Vector.h"

typedef void(*ListenerCallback)(int argc, void* argv);

typedef struct {
  ListenerCallback callback;
  String event_type;
} Listener;

DECLARE_VECTORS(Listener);

Listener Listener_create(ListenerCallback callback, String event_type);

#endif // LISTENER_H