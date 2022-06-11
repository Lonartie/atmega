#include "Actor.h"
#include "Vector.h"

typedef void* void_ptr;

DECLARE_VECTOR(void_ptr);
DEFINE_VECTOR(void_ptr);

static Vector_void_ptr actor_instance;
static bool initialized = false;

void* set_actor(void* obj) 
{
  if (!initialized) {
    initialized = true;
    actor_instance = Vector_void_ptr_create();
  }

  Vector_void_ptr_push_back(&actor_instance, obj);
  return obj;
}

void* get_actor() 
{
  return Vector_void_ptr_get(&actor_instance, actor_instance.size - 1);
}

void release_actor()
{
  Vector_void_ptr_erase(&actor_instance, actor_instance.size - 1);
}