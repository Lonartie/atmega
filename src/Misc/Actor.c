#include "Actor.h"
#include "Vector.h"

typedef void* void_ptr;

static void* actor_instances[64];
static uint actors_count = 0;

void set_actor(void* obj) 
{
  actor_instances[actors_count++] = obj;
}

void* get_actor() 
{
  return actor_instances[actors_count - 1];
}

void release_actor()
{
  actors_count--;
}