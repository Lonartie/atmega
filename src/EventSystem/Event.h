#ifndef EVENT_H
#define EVENT_H

#include "String.h"
#include "../Misc/Vector.h"

typedef struct Event
{
  String event_type;
} Event;

DECLARE_VECTORS(Event);

Event Event_create(String event_type);

#endif // EVENT_H