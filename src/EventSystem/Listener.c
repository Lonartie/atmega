#include "Listener.h"

DEFINE_VECTORS(Listener);

Listener Listener_create_r(void* receiver, ListenerReceiverCallback callback, String event_type)
{
  Listener listener;
  listener.receiver = receiver;
  listener.callback = NULL;
  listener.callback_r = callback;
  listener.event_type = event_type;
  return listener;
}

Listener Listener_create(ListenerCallback callback, String event_type)
{
  Listener listener;
  listener.receiver = NULL;
  listener.callback = callback;
  listener.callback_r = NULL;
  listener.event_type = event_type;
  return listener;
}
