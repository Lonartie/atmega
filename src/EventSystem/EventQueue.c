#include "EventQueue.h"

#include <stdbool.h>

EventQueue* EventQueue_instance()
{
  static EventQueue instance;
  static bool initialized = false;

  if (!initialized) {
    instance.callbacks = Vector_Callback_create();
    initialized = true;
  }

  return &instance;
}

void EventQueue_register_callback(String event_type, CallbackFunction fun)
{
  EventQueue* event_queue = EventQueue_instance();
  Callback callback = { fun, event_type };
  Vector_Callback_push_back(&event_queue->callbacks, callback);
}

void EventQueue_send_event(String event_type, uint32_t argc, void** argv)
{
  EventQueue* event_queue = EventQueue_instance();
  for (uint32_t i = 0; i < event_queue->callbacks.size; i++) {
    Callback* callback = &event_queue->callbacks.data[i];
    if (String_equals(callback->event_type, event_type)) {
      callback->function(argc, argv);
    }
  }
}