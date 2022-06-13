#include "Listener.h"

DEFINE_VECTORS(Listener);

Listener Listener_create_with_receiver(void* receiver, ListenerReceiverCallback callback, String event_type)
{
  Listener listener;
  listener.receiver = receiver;
  listener.callback_with_receiver = callback;
  listener.event_type = event_type;
  return listener;
}

Listener Listener_create(ListenerCallback callback, String event_type)
{
  Listener listener;
  listener.receiver = NULL;
  listener.callback = callback;
  listener.event_type = event_type;
  return listener;
}