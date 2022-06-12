#include "../EventSystem/EventQueue.h"
#include "../Misc/Actor.h"
#include "../Misc/utils.h"
#include <stdio.h>

void callme(int argc, void* argv)
{
  printf("argc: %d\n", argc);
  int* argv_ = (int*)argv;
  for (int i = 0; i < argc; i++)
  {
    printf("argv[%d]: %d\n", i, argv_[i]);
  }
}

int main()
{
  EventQueue_reg_listener(EventQueue_instance(), Listener_create(callme, "test"));

  Vector_int_4 vec = Vector_int_4_create();
  Vector_int_4_push_back(&vec, 0);
  Vector_int_4_push_back(&vec, 1);
  Vector_int_4_push_back(&vec, 2);
  Event event = Event_create("test", vec.size, vec.data);

  EventQueue_send_event(EventQueue_instance(), event);
  EventQueue_run(EventQueue_instance());
  return 0;
}