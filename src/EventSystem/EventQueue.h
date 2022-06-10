#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Callback.h"
#include "String.h"

typedef struct
{
  Vector_Callback callbacks;
} EventQueue;

EventQueue* EventQueue_instance();
void EventQueue_register_callback(String event_type, CallbackFunction fun);
void EventQueue_send_event(String event_type, uint32_t argc, void** argv);

#endif // EVENT_QUEUE_H