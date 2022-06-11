#ifndef ACTOR_H
#define ACTOR_H

void* set_actor(void* obj);
void* get_actor();
void release_actor();

#define DECLARE_ACTOR_FUNC(TYPE) \
TYPE* TYPE##_set_actor(TYPE* obj);

#define DEFINE_ACTOR_FUN(TYPE) \
TYPE* TYPE##_set_actor(TYPE* obj) { \
  set_actor(obj); \
  return obj; \
}

#define UNPACK(...) __VA_ARGS__

#define DECLARE_CTX_FUNC(TYPE) \
TYPE* TYPE##_actor();

#define DEFINE_CTX_FUN(TYPE) \
TYPE* TYPE##_actor() { \
  return (TYPE*) get_actor(); \
}

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

#define DECLARE_ACTOR(TYPE) \
DECLARE_ACTOR_FUNC(TYPE) \
DECLARE_CTX_FUNC(TYPE) \
DECLARE_RELEASE_FUNC(TYPE)

#define DEFINE_ACTOR(TYPE) \
DEFINE_ACTOR_FUN(TYPE) \
DEFINE_CTX_FUN(TYPE) \
DEFINE_RELEASE_FUN(TYPE)

#define DECLARE_ACTOR_MEM(TYPE) \
struct TYPE* (*actor)(); \
void (*release_actor)();

#define SET_ACTOR_MEM(OBJ, TYPE) \
OBJ.actor = TYPE##_actor; \
OBJ.release_actor = TYPE##_release_actor;

#define SET_ACTOR_FOWARDER(OBJ, TYPE, NAME) \
OBJ.NAME = TYPE##_##NAME##_actor;

#define ACTOR_SCOPE_IMPL \
bool after_scope() { \
  release_actor(); \
  return false; \
} \
for (int __index__ = 0; __index__ != 1 || after_scope(); __index__++)

#define ACTOR_SCOPE(OBJ) \
set_actor(&OBJ); \
ACTOR_SCOPE_IMPL

#define ACTOR_SCOPE_PTR(OBJ) \
set_actor(OBJ); \
ACTOR_SCOPE_IMPL


#endif // ACTOR_H