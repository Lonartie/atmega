#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "../Misc/String.h"
#include "Event.h"
#include "Listener.h"
#include "Updater.h"

/// @brief the event system that can be used to send events and listen for them
/// @note this is designed to be used as a singleton with
/// 'EventSystem_instance()'
typedef struct EventSystem {
  Vector_Updater_16 updaters;
  Vector_Listener_64 listeners;
  bool exit_flag;
} EventSystem;

/// @returns the event system singleton instance
EventSystem* EventSystem_instance();

/// @brief registers the given updater to the event system
void EventSystem_reg_updater(EventSystem* _this, Updater updater);

/// @brief unregisters the given updater from the event system
void EventSystem_unreg_updater(EventSystem* _this, Updater updater);

/// @brief registers the given listener to the event system
void EventSystem_reg_listener(EventSystem* _this, Listener listener);

/// @brief unregisters the given listener from the event system
void EventSystem_unreg_listener(EventSystem* _this, Listener listener);

/// @brief sends the given event to the event system
void EventSystem_send_event(EventSystem* _this, Event event);

/// @brief runs the event system
void EventSystem_run(EventSystem* _this);

/// @brief exits the event system
void EventSystem_exit(EventSystem* _this);

#endif  // EVENT_QUEUE_H