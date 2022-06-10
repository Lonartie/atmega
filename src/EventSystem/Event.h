#ifndef EVENT_H
#define EVENT_H

#include "String.h"
#include "Vector.h"

typedef void(*Cleanup)(void*);
typedef struct 
{
  String event_type;
  int argc;
  void** argv;
  Cleanup cleaner;
} Event;

DECLARE_VECTOR(Event);

Event Event_create(String event_type, int argc, void** argv);

#endif // EVENT_H