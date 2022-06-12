#include "Event.h"

DEFINE_VECTORS(Event);

Event Event_create(String event_type)
{
  Event event;
  event.event_type = event_type;
  return event;
}