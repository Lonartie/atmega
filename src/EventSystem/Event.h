#ifndef EVENT_H
#define EVENT_H

#include "../Misc/String.h"
#include "../Misc/Vector.h"

/// @brief the event type that can be send to the event system
/// @note in the future this may contain additional event data
typedef struct Event
{
  String event;
} Event;

DECLARE_VECTORS(Event);

/// @brief creates a new event with the specified event name
/// @returns a new event with the specified event name
Event Event_create(String event);

#endif // EVENT_H