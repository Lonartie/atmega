#ifndef LISTENER_H
#define LISTENER_H

#include "Event.h"
#include "../Misc/utils.h"
#include "../Misc/String.h"

typedef void(*ListenerReceiverCallback)(void*);
typedef void(*ListenerCallback)();

typedef struct {
  ListenerReceiverCallback callback_r;
  ListenerCallback callback;
  String event_type;
  void* receiver;
} Listener;

DECLARE_VECTORS(Listener);

Listener Listener_create_r(void* receiver, ListenerReceiverCallback callback, String event_type);
Listener Listener_create(ListenerCallback callback, String event_type);

#endif // LISTENER_H