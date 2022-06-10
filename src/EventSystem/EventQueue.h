#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include "String.h"
#include "Listener.h"
#include "Updater.h"

typedef struct
{
  Vector_Updater updaters;
  Vector_Event events;
  Vector_Listener listeners;
} EventQueue;

EventQueue* EventQueue_instance();
void EventQueue_register_updater(Updater updater);
void EventQueue_register_listener(Listener listener);
void EventQueue_send_event(Event event);
void EventQueue_run();

#endif // EVENT_QUEUE_H