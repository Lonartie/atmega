#include "Actor.h"
#include "Vector.h"

typedef void* void_ptr;

static void* actor_instances[64];
static uint actors_count = 0;
static bool initialized = false;

void* set_actor(void* obj) 
{
  if (!initialized) {
    initialized = true;
  }

  actor_instances[actors_count++] = obj;
  return obj;
}

void* get_actor() 
{
  return actor_instances[actors_count - 1];
}

void release_actor()
{
  actors_count--;
}