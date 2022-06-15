#ifndef LISTENER_H
#define LISTENER_H

#include "Event.h"
#include "../Misc/utils.h"
#include "../Misc/String.h"

typedef void(*ListenerReceiverCallback)(void*);
typedef void(*ListenerCallback)();

/// @brief a listener that can be added to the event system to listen for events
typedef struct Listener {
  ListenerReceiverCallback callback_r;
  ListenerCallback callback;
  String event;
  void* receiver;
} Listener;

DECLARE_VECTORS(Listener);

/// @brief creates a new listener with a receiver listening to the specified event
/// @param receiver the receiver that will be passed to the callback function
/// @param callback the callback function that will be called when the event is received
/// @param event the event that the listener will listen for
Listener Listener_create_r(void* receiver, ListenerReceiverCallback callback, String event);

/// @brief creates a new listener listening to the specified event
/// @param callback the callback function that will be called when the event is received
/// @param event the event that the listener will listen for
Listener Listener_create(ListenerCallback callback, String event);

#endif // LISTENER_H