#ifndef EVENT_H
#define EVENT_H

#include "../Misc/String.h"
#include "../Misc/Vector.h"

typedef struct Event
{
  String event;
} Event;

DECLARE_VECTORS(Event);

Event Event_create(String event);

#endif // EVENT_H