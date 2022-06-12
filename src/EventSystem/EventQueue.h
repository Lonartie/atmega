#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include "String.h"
#include "Listener.h"
#include "Updater.h"

typedef struct EventQueue {
  Vector_Updater_16 updaters;
  Vector_Event_32 events;
  Vector_Listener_64 listeners;

  void(*reg_updater)(Updater);
  void(*unreg_updater)(Updater);
  void(*reg_listener)(Listener);
  void(*unreg_listener)(Listener);
  void(*send_event)(Event);
  void(*run)();
} EventQueue;

EventQueue* EventQueue_instance();
void EventQueue_reg_updater(EventQueue* _this, Updater updater);
void EventQueue_unreg_updater(EventQueue* _this, Updater updater);
void EventQueue_reg_listener(EventQueue* _this, Listener listener);
void EventQueue_unreg_listener(EventQueue* _this, Listener listener);
void EventQueue_send_event(EventQueue* _this, Event event);
void EventQueue_run(EventQueue* _this);

#endif // EVENT_QUEUE_H