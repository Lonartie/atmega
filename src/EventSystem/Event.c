#include "Event.h"

DEFINE_VECTORS(Event);

Event Event_create(String event_type, int argc, void* argv)
{
  Event event;
  event.event_type = event_type;
  event.argc = argc;
  event.argv = argv;
  event.cleaner = NULL;
  return event;
}

Event Event_create_with_cleaner(String event_type, int argc, void* argv, 
  void(*cleaner)(struct Event*))
{
  Event event;
  event.event_type = event_type;
  event.argc = argc;
  event.argv = argv;
  event.cleaner = cleaner;
  return event;
}