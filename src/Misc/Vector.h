#ifndef VECTOR_H
#define VECTOR_H

#include "utils.h"

#define DECLARE_VECTOR(TYPE, SIZE) \
typedef struct Vector_##TYPE##_##SIZE \
{ \
  uint64_t size; \
  uint64_t capacity; \
  TYPE data[SIZE]; \
  void(*destroy)(); \
  struct Vector_##TYPE##_##SIZE(*copy)(); \
  struct Vector_##TYPE##_##SIZE(*move)(); \
  struct Vector_##TYPE##_##SIZE(*sublist)(uint64_t, uint64_t); \
} Vector_##TYPE##_##SIZE; \
\
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_create(); \
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_copy(Vector_##TYPE##_##SIZE* other); \
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_move(Vector_##TYPE##_##SIZE* other); \
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_sublist(Vector_##TYPE##_##SIZE* other, uint64_t start, uint64_t end); \
void Vector_##TYPE##_##SIZE##_clear(Vector_##TYPE##_##SIZE* vector); \
TYPE* Vector_##TYPE##_##SIZE##_get(Vector_##TYPE##_##SIZE* vector, uint64_t index); \
void Vector_##TYPE##_##SIZE##_set(Vector_##TYPE##_##SIZE* vector, uint64_t index, TYPE value); \
void Vector_##TYPE##_##SIZE##_erase(Vector_##TYPE##_##SIZE* vector, uint64_t index); \
void Vector_##TYPE##_##SIZE##_push_back(Vector_##TYPE##_##SIZE* vector, TYPE value);

#define DEFINE_VECTOR(TYPE, SIZE) \
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_create() \
{ \
  Vector_##TYPE##_##SIZE vector; \
  vector.size = 0; \
  vector.capacity = SIZE; \
  return vector; \
} \
\
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_copy(Vector_##TYPE##_##SIZE* other) \
{ \
  Vector_##TYPE##_##SIZE vector; \
  vector.size = other->size; \
  vector.capacity = SIZE; \
  memcpy(vector.data, other->data, sizeof(TYPE) * vector.size); \
  return vector; \
} \
\
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_move(Vector_##TYPE##_##SIZE* other) \
{ \
  Vector_##TYPE##_##SIZE vector; \
  vector.size = other->size; \
  vector.capacity = SIZE; \
  memcpy(vector.data, other->data, sizeof(TYPE) * vector.size); \
  other->size = 0; \
  other->capacity = SIZE; \
  return vector; \
} \
\
Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_sublist(Vector_##TYPE##_##SIZE* other, uint64_t start, uint64_t end) \
{ \
  Vector_##TYPE##_##SIZE vector; \
  vector.size = end - start; \
  vector.capacity = SIZE; \
  memcpy(vector.data, other->data + start, sizeof(TYPE) * vector.size); \
  return vector; \
} \
\
void Vector_##TYPE##_##SIZE##_clear(Vector_##TYPE##_##SIZE* vector) \
{ \
  vector->size = 0; \
} \
\
TYPE* Vector_##TYPE##_##SIZE##_get(Vector_##TYPE##_##SIZE* vector, uint64_t index) \
{ \
  return vector->data + index; \
} \
\
void Vector_##TYPE##_##SIZE##_set(Vector_##TYPE##_##SIZE* vector, uint64_t index, TYPE value) \
{ \
  vector->data[index] = value; \
} \
\
void Vector_##TYPE##_##SIZE##_erase(Vector_##TYPE##_##SIZE* vector, uint64_t index) \
{ \
  memmove(vector->data + index, vector->data + index + 1, sizeof(TYPE) * (vector->size - index - 1)); \
  vector->size--; \
} \
\
void Vector_##TYPE##_##SIZE##_push_back(Vector_##TYPE##_##SIZE* vector, TYPE value) \
{ \
  vector->data[vector->size] = value; \
  vector->size++; \
}

#define DECLARE_VECTORS(TYPE) \
DECLARE_VECTOR(TYPE, 4) \
DECLARE_VECTOR(TYPE, 8) \
DECLARE_VECTOR(TYPE, 16) \
DECLARE_VECTOR(TYPE, 32) \
DECLARE_VECTOR(TYPE, 64)

#define DEFINE_VECTORS(TYPE) \
DEFINE_VECTOR(TYPE, 4) \
DEFINE_VECTOR(TYPE, 8) \
DEFINE_VECTOR(TYPE, 16) \
DEFINE_VECTOR(TYPE, 32) \
DEFINE_VECTOR(TYPE, 64)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

DECLARE_VECTORS(char)
DECLARE_VECTORS(uchar)
DECLARE_VECTORS(short)
DECLARE_VECTORS(ushort)
DECLARE_VECTORS(int)
DECLARE_VECTORS(uint)
DECLARE_VECTORS(long)
DECLARE_VECTORS(ulong)
DECLARE_VECTORS(bool)
DECLARE_VECTORS(float)
DECLARE_VECTORS(double)

#endif