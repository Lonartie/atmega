#include "EventQueue.h"

#include <stdbool.h>

EventQueue* EventQueue_instance()
{
  static EventQueue instance;
  static bool initialized = false;

  if (!initialized) {
    instance.callbacks = Vector_Vector_Callback_create();
    instance.event_types = Vector_String_create();
  }

  return &instance;
}

void EventQueue_register_event_type(String event_type)
{
  EventQueue* event_queue = EventQueue_instance();
  Vector_String_push_back(&event_queue->event_types, event_type);
}

uint64_t EventQueue_get_event_type_index(String event_type)
{
  EventQueue* event_queue = EventQueue_instance();
  for (uint64_t i = 0; i < event_queue->event_types.size; i++)
    if (String_equals(event_queue->event_types.data[i], event_type))
      return i;
  abort();
}

void EventQueue_register_callback(String event_type, Callback callback)
{
  EventQueue* event_queue = EventQueue_instance();
  uint64_t event_index = EventQueue_get_event_type_index(event_type);
  if (event_queue->callbacks.size <= event_index)
  {
    Vector_Vector_Callback_resize(&event_queue->callbacks, event_index + 1);
    event_queue->callbacks.data[event_index] = Vector_Callback_create();
  }
  Vector_Callback_push_back(&event_queue->callbacks.data[event_index], callback);
}

void EventQueue_send_event(String event_type, uint32_t argc, void** argv)
{
  EventQueue* event_queue = EventQueue_instance();
  uint64_t event_index = EventQueue_get_event_type_index(event_type);
  Vector_Callback callbacks = event_queue->callbacks.data[event_index];

  for (uint64_t i = 0; i < callbacks.size; i++)
    callbacks.data[i](argc, argv);
}