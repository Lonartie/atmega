#ifndef LISTENER_H
#define LISTENER_H

#include "../Misc/String.h"
#include "../Misc/Vector.h"

/// @brief callback type for a listener that receives data
typedef void (*ListenerDataCallback)(void*);

/// @brief callback type for a listener that doesn't receive data
typedef void (*ListenerCallback)();

/// @brief a listener that can be added to the event system to listen for
/// events. It contains either a callback with data or a callback without data.
typedef struct Listener {
  ListenerDataCallback callback_d;
  ListenerCallback callback;
  String event;
  void* data;
} Listener;

DECLARE_VECTORS(Listener);

/// @brief creates a new listener with a data listening to the specified
/// event
/// @param data the data that will be passed to the callback function
/// @param callback the callback function that will be called when the event is
/// received
/// @param event the event that the listener will listen for
Listener Listener_create_r(void* data, ListenerDataCallback callback,
                           String event);

/// @brief creates a new listener listening to the specified event
/// @param callback the callback function that will be called when the event is
/// received
/// @param event the event that the listener will listen for
Listener Listener_create(ListenerCallback callback, String event);

#endif  // LISTENER_H