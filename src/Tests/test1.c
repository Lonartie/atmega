#include "../EventSystem/EventSystem.h"
#include "../Misc/Actor.h"
#include "../Misc/utils.h"
#include <stdio.h>

void callme(uint8_t data[8])
{
  for (int i = 0; i < 8; i++)
    printf("%d\n", data[i]);

  ACTOR(*EventSystem_instance()).exit();
}

int main()
{
  EventSystem* eq = EventSystem_instance();
  ACTOR(*eq).reg_listener(Listener_create_d(callme, "test"));

  Event event = Event_create_d("test", (uint8_t[9]){1, 2, 3, 4, 5, 6, 7, 8, 9});

  ACTOR_SCOPE(*eq)
  {
    eq->send_event(event);
    eq->run();
  }

  return 0;
}