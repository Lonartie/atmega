#include "Event.h"

DEFINE_VECTOR(Event);

Event Event_create(String event_type, int argc, void** argv)
{
  Event event;
  event.event_type = event_type;
  event.argc = argc;
  event.argv = argv;
  event.cleaner = NULL;
  return event;
}