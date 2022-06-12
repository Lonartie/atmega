#ifndef EVENT_H
#define EVENT_H

#include "String.h"
#include "../Misc/Vector.h"

#define MAX(A, B) ((A) > (B) ? (A) : (B))

typedef uchar event_data_t[MAX(sizeof(int) * 2, sizeof(void*))];

typedef struct Event
{
  String event_type;
  event_data_t event_data;
} Event;

DECLARE_VECTORS(Event);

Event Event_create(String event_type);

#endif // EVENT_H