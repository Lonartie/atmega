#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include "Listener.h"
#include "Updater.h"

/// @brief the event system that can be used to send events and listen for them
/// @note this is designed to be used as a singleton with
/// 'EventSystem_instance()'
typedef struct EventSystem {
  Vector_Updater_8 updaters;
  Vector_Listener_8 listeners;
  bool exit_flag;
} EventSystem;

/// @returns the event system singleton instance
EventSystem* EventSystem_instance();

/// @brief registers the given updater to the event system
/// @param _this the event system to register the updater to
/// @param updater the updater to register to the event system
void EventSystem_reg_updater(EventSystem* _this, Updater updater);

/// @brief unregisters the given updater from the event system
/// @param _this the event system to unregister the updater from
/// @param updater the updater to unregister from the event system
void EventSystem_unreg_updater(EventSystem* _this, Updater updater);

/// @brief registers the given listener to the event system
/// @param _this the event system to register the listener to
/// @param listener the listener to register to the event system
void EventSystem_reg_listener(EventSystem* _this, Listener listener);

/// @brief unregisters the given listener from the event system
/// @param _this the event system to unregister the listener from
/// @param listener the listener to unregister from the event system
void EventSystem_unreg_listener(EventSystem* _this, Listener listener);

/// @brief sends the given event to the event system
/// @param _this the event system to send the event to
/// @param event the event to send to the event system
void EventSystem_send_event(EventSystem* _this, Event event);

/// @brief runs the event system
/// @param _this the event system to run
void EventSystem_run(EventSystem* _this);

/// @brief exits the event system
/// @param _this the event system to exit
void EventSystem_exit(EventSystem* _this);

#endif  // EVENT_QUEUE_H