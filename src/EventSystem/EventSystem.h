#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include "Listener.h"
#include "Updater.h"
#include "../Misc/String.h"

typedef struct EventSystem {
  Vector_Updater_16 updaters;
  Vector_Listener_64 listeners;
  bool exit_flag;

  void(*reg_updater)(Updater);
  void(*unreg_updater)(Updater);
  void(*reg_listener)(Listener);
  void(*unreg_listener)(Listener);
  void(*send_event)(Event);
  void(*run)();
  void(*exit)();
} EventSystem;

EventSystem* EventSystem_instance();
void EventSystem_reg_updater(EventSystem* _this, Updater updater);
void EventSystem_unreg_updater(EventSystem* _this, Updater updater);
void EventSystem_reg_listener(EventSystem* _this, Listener listener);
void EventSystem_unreg_listener(EventSystem* _this, Listener listener);
void EventSystem_send_event(EventSystem* _this, Event event);
void EventSystem_run(EventSystem* _this);
void EventSystem_exit(EventSystem* _this);

#endif // EVENT_QUEUE_H