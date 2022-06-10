#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include "String.h"
#include "Listener.h"
#include "Updater.h"

typedef struct EventQueue {
  Vector_Updater updaters;
  Vector_Event events;
  Vector_Listener listeners;

  void(*reg_updater)(struct EventQueue*, Updater);
  void(*unreg_updater)(struct EventQueue*, Updater);
  void(*reg_listener)(struct EventQueue*, Listener);
  void(*unreg_listener)(struct EventQueue*, Listener);
  void(*send_event)(struct EventQueue*, Event);
  void(*run)(struct EventQueue*);
} EventQueue;

EventQueue* EventQueue_instance();
void EventQueue_register_updater(EventQueue* _this, Updater updater);
void EventQueue_unregister_updater(EventQueue* _this, Updater updater);
void EventQueue_register_listener(EventQueue* _this, Listener listener);
void EventQueue_unregister_listener(EventQueue* _this, Listener listener);
void EventQueue_send_event(EventQueue* _this, Event event);
void EventQueue_run(EventQueue* _this);

#endif // EVENT_QUEUE_H