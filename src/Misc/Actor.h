#ifndef ACTOR_H
#define ACTOR_H

/// @brief sets the given object as the current actor to call 'member-functions' on
/// @note the actor system is queue-based
/// @param object the object to set as the current actor
void set_actor(void* obj);

/// @returns the current actor to call 'member-functions' on
void* get_actor();

/// @brief releases the current actor
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

#define _CAT3(A, B, C) A##B##C
#define CAT3(A, B, C) _CAT3(A, B, C)

#define ACTOR_SCOPE_IMPL \
for (int CAT3(__index_,__LINE__,__) = 0; \
  CAT3(__index_,__LINE__,__) != 1 || (release_actor(), false); \
  CAT3(__index_,__LINE__,__)++)

#define ACTOR_SCOPE(OBJ) \
set_actor(&OBJ); \
ACTOR_SCOPE_IMPL

#define ACTORP_SCOPE(OBJ) \
set_actor(OBJ); \
ACTOR_SCOPE_IMPL

#define ACTOR(OBJ) \
ACTOR_SCOPE(OBJ) (OBJ)

#define ACTORP(OBJ) \
ACTORP_SCOPE(OBJ) (OBJ)

#endif // ACTOR_H