#include "Listener.h"

DEFINE_VECTORS(Listener);

Listener Listener_create(ListenerCallback callback, String event_type)
{
  Listener listener;
  listener.callback = callback;
  listener.event_type = event_type;
  return listener;
}