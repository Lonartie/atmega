#include "EventQueue.h"

#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static const uint64_t EVENT_PERIOD_US = 100;

EventQueue* EventQueue_instance()
{
  static EventQueue instance;
  static bool initialized = false;

  if (!initialized) {
    instance.updaters = Vector_Updater_create();
    instance.events = Vector_Event_create();
    instance.listeners = Vector_Listener_create();
    initialized = true;
  }

  return &instance;
}

void EventQueue_register_updater(Updater updater)
{
  EventQueue* event_queue = EventQueue_instance();
  Vector_Updater_push_back(&event_queue->updaters, updater);
}

void EventQueue_register_listener(Listener listener)
{
  EventQueue* event_queue = EventQueue_instance();
  Vector_Listener_push_back(&event_queue->listeners, listener);
}

void EventQueue_send_event(Event event)
{
  EventQueue* event_queue = EventQueue_instance();
  Vector_Event_push_back(&event_queue->events, event);
}

void EventQueue_run()
{
  EventQueue* event_queue = EventQueue_instance();
  while (1)
  {
    _delay_us(EVENT_PERIOD_US);

    // run updaters
    for (uint32_t i = 0; i < event_queue->updaters.size; i++) {
      event_queue->updaters.data[i].update(event_queue->updaters.data[i].object);
    }

    Vector_Event tmp_events = Vector_Event_move(&event_queue->events);

    // take all events
    for (uint64_t i = 0; i < tmp_events.size; ++i) {
      Event event = tmp_events.data[i];

      // find listeners for this event
      for (uint64_t j = 0; j < event_queue->listeners.size; ++j) {
        if (String_equals(event.event_type, event_queue->listeners.data[j].event_type)) {

          // call listener
          event_queue->listeners.data[j].callback(event.argc, event.argv);
        }
      }

      event.cleaner(&event);
    }

    Vector_Event_destroy(&tmp_events);
  }
}