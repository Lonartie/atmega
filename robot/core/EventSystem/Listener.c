#include "Listener.h"

DEFINE_VECTORS(Listener);

Listener Listener_create_r(void* data, ListenerDataCallback callback,
                           String event_type) {
  Listener listener;
  listener.data = data;
  listener.callback = NULL;
  listener.callback_d = callback;
  listener.event = event_type;
  return listener;
}

Listener Listener_create(ListenerCallback callback, String event_type) {
  Listener listener;
  listener.data = NULL;
  listener.callback = callback;
  listener.callback_d = NULL;
  listener.event = event_type;
  return listener;
}
