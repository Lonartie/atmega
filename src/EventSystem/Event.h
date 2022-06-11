#ifndef EVENT_H
#define EVENT_H

#include "String.h"
#include "../Misc/Vector.h"

typedef struct Event
{
  String event_type;
  int argc;
  void** argv;
  void(*cleaner)(struct Event*);
} Event;

DECLARE_VECTOR(Event);

Event Event_create(String event_type, int argc, void** argv);
Event Event_create_with_cleaner(String event_type, int argc, void** argv, void(*cleaner)(struct Event*));

#endif // EVENT_H