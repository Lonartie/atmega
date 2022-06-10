#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Callback.h"
#include "String.h"

typedef struct
{
  Vector_Vector_Callback callbacks;
  Vector_String event_types;
} EventQueue;

EventQueue* EventQueue_instance();
void EventQueue_register_event_type(String event_type);
uint64_t EventQueue_get_event_type_index(String event_type);
void EventQueue_register_callback(String event_type, Callback callback);
void EventQueue_send_event(String event_type, uint32_t argc, void** argv);

#endif // EVENT_QUEUE_H