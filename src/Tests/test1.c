#include "../EventSystem/EventQueue.h"
#include "../Misc/Actor.h"
#include "../Misc/utils.h"
#include <stdio.h>

void callme()
{
}

int main()
{
  EventQueue* eq = EventQueue_instance();
  ACTOR(*eq).reg_listener(Listener_create(callme, "test"));

  Event event = Event_create("test");

  ACTOR(*eq).send_event(event);
  ACTOR(*eq).run();
  return 0;
}