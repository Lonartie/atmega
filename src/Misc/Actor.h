#ifndef ACTOR_H
#define ACTOR_H

void* set_actor(void* obj);
void* get_actor();
void release_actor();

#define UNPACK(...) __VA_ARGS__

#define DECLARE_RELEASE_FUNC(TYPE) \
void TYPE##_release_actor();

#define DEFINE_RELEASE_FUN(TYPE) \
void TYPE##_release_actor() { \
  release_actor(); \
}

#define DECLARE_ACTOR_FORWARDER_N(RET, TYPE, NAME, PARAMS) \
RET TYPE##_##NAME##_actor PARAMS;

#define DECLARE_ACTOR_FORWARDER(RET, TYPE, NAME) \
RET TYPE##_##NAME##_actor();

#define DEFINE_ACTOR_FORWARDER_N(RET, TYPE, NAME, PARAMS, ARGS) \
RET TYPE##_##NAME##_actor PARAMS { \
  return TYPE##_##NAME((TYPE*) get_actor(), UNPACK ARGS); \
}

#define DEFINE_ACTOR_FORWARDER(RET, TYPE, NAME) \
RET TYPE##_##NAME##_actor() { \
  return TYPE##_##NAME((TYPE*) get_actor()); \
}

#define SET_ACTOR_FORWARDER(OBJ, TYPE, NAME) \
OBJ.NAME = TYPE##_##NAME##_actor;

#define ACTOR_SCOPE_IMPL \
for (int __index__ = 0; __index__ != 1 || (release_actor(), false); __index__++)

#define ACTOR_SCOPE(OBJ) \
set_actor(&OBJ); \
ACTOR_SCOPE_IMPL

#define ACTOR_SCOPE_PTR(OBJ) \
set_actor(OBJ); \
ACTOR_SCOPE_IMPL

#define ACTOR(OBJ) \
ACTOR_SCOPE(OBJ) (OBJ)


#endif // ACTOR_H