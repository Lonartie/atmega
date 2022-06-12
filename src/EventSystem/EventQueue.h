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

  void(*reg_updater)(Updater);
  void(*unreg_updater)(Updater);
  void(*reg_listener)(Listener);
  void(*unreg_listener)(Listener);
  void(*send_event)(Event);
  void(*run)();
  DECLARE_ACTOR_MEM(EventQueue);
} EventQueue;

DECLARE_ACTOR(EventQueue);

EventQueue* EventQueue_instance();
void EventQueue_reg_updater(EventQueue* _this, Updater updater);
void EventQueue_unreg_updater(EventQueue* _this, Updater updater);
void EventQueue_reg_listener(EventQueue* _this, Listener listener);
void EventQueue_unreg_listener(EventQueue* _this, Listener listener);
void EventQueue_send_event(EventQueue* _this, Event event);
void EventQueue_run(EventQueue* _this);

#endif // EVENT_QUEUE_H