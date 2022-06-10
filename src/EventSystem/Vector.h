#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define DISABLE_WARNING(X)            \
_Pragma("GCC diagnostic push")        \
_Pragma(#X)

#define WARNING_POP                   \
_Pragma("GCC diagnostic pop")

#define PROMOTE(type) _Generic((type),        \
  bool    : int32_t,                          \
  int8_t  : int32_t,                          \
  uint8_t : int32_t,                          \
  int16_t : int32_t,                          \
  uint16_t: int32_t                           \
)

#define DECLARE_VECTOR(type)                                                                \
typedef struct Vector_##type                                                                \
{                                                                                           \
  uint64_t size;                                                                            \
  uint64_t capacity;                                                                        \
  type* data;                                                                               \
  void(*destroy)(struct Vector_##type*);                                                    \
  struct Vector_##type(*sublist)(struct Vector_##type*, uint64_t, uint64_t);                \
  void(*resize)(struct Vector_##type*, uint64_t);                                           \
  void(*clear)(struct Vector_##type*);                                                      \
  type*(*get)(struct Vector_##type*, uint64_t);                                             \
  void(*set)(struct Vector_##type*, uint64_t, type);                                        \
  void(*erase)(struct Vector_##type*, uint64_t);                                            \
  void(*push_back)(struct Vector_##type*, type);                                            \
} Vector_##type;                                                                            \
                                                                                            \
Vector_##type Vector_##type##_create();                                                     \
Vector_##type Vector_##type##_create_with_capacity(uint64_t capacity);                      \
Vector_##type Vector_##type##_copy(Vector_##type* other);                                   \
Vector_##type Vector_##type##_move(Vector_##type* other);                                   \
void Vector_##type##_destroy(Vector_##type* vector);                                        \
Vector_##type Vector_##type##_sublist(Vector_##type* other, uint64_t start, uint64_t end);  \
void Vector_##type##_resize(Vector_##type* vector, uint64_t size);                          \
void Vector_##type##_clear(Vector_##type* vector);                                          \
type* Vector_##type##_get(Vector_##type* vector, uint64_t index);                           \
void Vector_##type##_set(Vector_##type* vector, uint64_t index, type value);                \
void Vector_##type##_erase(Vector_##type* vector, uint64_t index);                          \
void Vector_##type##_push_back(Vector_##type* vector, type value);

#define DEFINE_VECTOR(type)                                                                 \
Vector_##type Vector_##type##_create()                                                      \
{                                                                                           \
  Vector_##type vector;                                                                     \
  vector.size = 0;                                                                          \
  vector.capacity = 1;                                                                      \
  vector.data = malloc(sizeof(type) * vector.capacity);                                     \
  vector.destroy = Vector_##type##_destroy;                                                 \
  vector.sublist = Vector_##type##_sublist;                                                 \
  vector.resize = Vector_##type##_resize;                                                   \
  vector.clear = Vector_##type##_clear;                                                     \
  vector.get = Vector_##type##_get;                                                         \
  vector.set = Vector_##type##_set;                                                         \
  vector.erase = Vector_##type##_erase;                                                     \
  vector.push_back = Vector_##type##_push_back;                                             \
  return vector;                                                                            \
}                                                                                           \
                                                                                            \
Vector_##type Vector_##type##_create_with_capacity(uint64_t capacity)                       \
{                                                                                           \
  Vector_##type vector;                                                                     \
  vector.size = 0;                                                                          \
  vector.capacity = capacity;                                                               \
  vector.data = malloc(sizeof(type) * vector.capacity);                                     \
  vector.destroy = Vector_##type##_destroy;                                                 \
  vector.sublist = Vector_##type##_sublist;                                                 \
  vector.resize = Vector_##type##_resize;                                                   \
  vector.clear = Vector_##type##_clear;                                                     \
  vector.get = Vector_##type##_get;                                                         \
  vector.set = Vector_##type##_set;                                                         \
  vector.erase = Vector_##type##_erase;                                                     \
  return vector;                                                                            \
}                                                                                           \
                                                                                            \
Vector_##type Vector_##type##_copy(Vector_##type* other)                                    \
{                                                                                           \
  Vector_##type vector;                                                                     \
  vector.size = other->size;                                                                \
  vector.capacity = other->capacity;                                                        \
  vector.data = malloc(sizeof(type) * vector.capacity);                                     \
  vector.destroy = Vector_##type##_destroy;                                                 \
  vector.sublist = Vector_##type##_sublist;                                                 \
  vector.resize = Vector_##type##_resize;                                                   \
  vector.clear = Vector_##type##_clear;                                                     \
  vector.get = Vector_##type##_get;                                                         \
  vector.set = Vector_##type##_set;                                                         \
  vector.erase = Vector_##type##_erase;                                                     \
  memcpy(vector.data, other->data, sizeof(type) * vector.capacity);                         \
  return vector;                                                                            \
}                                                                                           \
                                                                                            \
Vector_##type Vector_##type##_move(Vector_##type* other)                                    \
{                                                                                           \
  Vector_##type vector;                                                                     \
  vector.size = other->size;                                                                \
  vector.capacity = other->capacity;                                                        \
  vector.data = other->data;                                                                \
  vector.destroy = Vector_##type##_destroy;                                                 \
  vector.sublist = Vector_##type##_sublist;                                                 \
  vector.resize = Vector_##type##_resize;                                                   \
  vector.clear = Vector_##type##_clear;                                                     \
  vector.get = Vector_##type##_get;                                                         \
  vector.set = Vector_##type##_set;                                                         \
  vector.erase = Vector_##type##_erase;                                                     \
  other->size = 0;                                                                          \
  other->capacity = 1;                                                                      \
  other->data = malloc(sizeof(type) * other->capacity);                                     \
  return vector;                                                                            \
}                                                                                           \
                                                                                            \
void Vector_##type##_destroy(Vector_##type* vector)                                         \
{                                                                                           \
  if (vector->data)                                                                         \
  {                                                                                         \
    free(vector->data);                                                                     \
  }                                                                                         \
}                                                                                           \
                                                                                            \
Vector_##type Vector_##type##_sublist(Vector_##type* other, uint64_t start, uint64_t end)   \
{                                                                                           \
  Vector_##type vector;                                                                     \
  vector.size = end - start;                                                                \
  vector.capacity = vector.size;                                                            \
  vector.data = malloc(sizeof(type) * vector.capacity);                                     \
  memcpy(vector.data, other->data + start, sizeof(type) * vector.capacity);                 \
  return vector;                                                                            \
}                                                                                           \
                                                                                            \
void Vector_##type##_resize(Vector_##type* vector, uint64_t size)                           \
{                                                                                           \
  if (vector->capacity < size)                                                              \
  {                                                                                         \
    vector->capacity = size;                                                                \
    vector->data = realloc(vector->data, sizeof(type) * vector->capacity);                  \
  }                                                                                         \
  vector->size = size;                                                                      \
}                                                                                           \
                                                                                            \
void Vector_##type##_clear(Vector_##type* vector)                                           \
{                                                                                           \
  vector->size = 0;                                                                         \
}                                                                                           \
                                                                                            \
type* Vector_##type##_get(Vector_##type* vector, uint64_t index)                            \
{                                                                                           \
  if (index >= vector->size)                                                                \
  {                                                                                         \
    return NULL;                                                                            \
  }                                                                                         \
  return vector->data + index;                                                              \
}                                                                                           \
                                                                                            \
void Vector_##type##_set(Vector_##type* vector, uint64_t index, type value)                 \
{                                                                                           \
  if (index >= vector->size)                                                                \
  {                                                                                         \
    return;                                                                                 \
  }                                                                                         \
  vector->data[index] = value;                                                              \
}                                                                                           \
                                                                                            \
void Vector_##type##_erase(Vector_##type* vector, uint64_t index)                           \
{                                                                                           \
  if (index >= vector->size)                                                                \
  {                                                                                         \
    return;                                                                                 \
  }                                                                                         \
  memmove(vector->data + index, vector->data + index + 1,                                   \
          sizeof(type) * (vector->size - index - 1));                                       \
  vector->size--;                                                                           \
}                                                                                           \
                                                                                            \
void Vector_##type##_push_back(Vector_##type* vector, type value)                           \
{                                                                                           \
  if (vector->size >= vector->capacity)                                                     \
  {                                                                                         \
    vector->capacity *= 2;                                                                  \
    vector->data = realloc(vector->data, sizeof(type) * vector->capacity);                  \
  }                                                                                         \
  vector->data[vector->size] = value;                                                       \
  vector->size++;                                                                           \
}

DECLARE_VECTOR(int);
DECLARE_VECTOR(bool);
DECLARE_VECTOR(double);

#endif