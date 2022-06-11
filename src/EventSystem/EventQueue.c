#include "EventQueue.h"
#include "../Misc/utils.h"
#include <stdbool.h>
// #include <util/delay.h>

#define USE_EVENT_QUEUE_DELAY false
static const uint64_t EVENT_PERIOD_US MAYBE_UNUSED = 100;

EventQueue* EventQueue_instance()
{
  static EventQueue instance;
  static bool initialized = false;

  if (!initialized) 
  {
    instance.updaters = Vector_Updater_create();
    instance.events = Vector_Event_create();
    instance.listeners = Vector_Listener_create();

    instance.reg_updater = EventQueue_register_updater;
    instance.unreg_updater = EventQueue_unregister_updater;
    instance.reg_listener = EventQueue_register_listener;
    instance.unreg_listener = EventQueue_unregister_listener;
    instance.send_event = EventQueue_send_event;
    instance.run = EventQueue_run;

    initialized = true;
  }

  return &instance;
}

void EventQueue_register_updater(EventQueue* _this, Updater updater)
{
  Vector_Updater_push_back(&_this->updaters, updater);
}

void EventQueue_unregister_updater(EventQueue* _this, Updater updater)
{
  for (uint64_t i = 0; i < _this->updaters.size; i++)
    if (_this->updaters.data[i].update == updater.update && 
        _this->updaters.data[i].object == updater.object)
    {
      Vector_Updater_erase(&_this->updaters, i);
      return;
    }
}

void EventQueue_register_listener(EventQueue* _this, Listener listener)
{
  Vector_Listener_push_back(&_this->listeners, listener);
}

void EventQueue_unregister_listener(EventQueue* _this, Listener listener)
{
  for (uint64_t i = 0; i < _this->listeners.size; i++)
    if (_this->listeners.data[i].callback == listener.callback && 
        String_equals(_this->listeners.data[i].event_type, listener.event_type))
    {
      Vector_Listener_erase(&_this->listeners, i);
      return;
    }
}

void EventQueue_send_event(EventQueue* _this, Event event)
{
  Vector_Event_push_back(&_this->events, event);
}

void EventQueue_run(EventQueue* _this)
{
  while (true)
  {
#if USE_EVENT_QUEUE_DELAY == true
    _delay_us(EVENT_PERIOD_US);
#endif

    // run updaters
    for (uint32_t i = 0; i < _this->updaters.size; i++) 
    {
      _this->updaters.data[i].update(_this->updaters.data[i].object);
    }

    Vector_Event tmp_events = Vector_Event_move(&_this->events);

    // take all events
    for (uint64_t i = 0; i < tmp_events.size; ++i) 
    {
      Event event = tmp_events.data[i];

      // find listeners for this event
      for (uint64_t j = 0; j < _this->listeners.size; ++j) 
      {
        if (String_equals(event.event_type, _this->listeners.data[j].event_type)) 
        {
          _this->listeners.data[j].callback(event.argc, event.argv);
        }
      }

      // clean up event
      if (event.cleaner)
        event.cleaner(&event);
    }

    Vector_Event_destroy(&tmp_events);
  }
}