#ifndef VECTOR_H
#define VECTOR_H

#include "Misc/Utils.h"

#define VECTOR_SIZE_T uint16_t

#define DECLARE_VECTOR(TYPE, SIZE)                                            \
  typedef struct Vector_##TYPE##_##SIZE {                                     \
    VECTOR_SIZE_T size;                                                       \
    VECTOR_SIZE_T capacity;                                                   \
    TYPE data[SIZE];                                                          \
    void (*destroy)();                                                        \
    struct Vector_##TYPE##_##SIZE (*copy)();                                  \
    struct Vector_##TYPE##_##SIZE (*move)();                                  \
    struct Vector_##TYPE##_##SIZE (*sublist)(VECTOR_SIZE_T, VECTOR_SIZE_T);   \
  } Vector_##TYPE##_##SIZE;                                                   \
                                                                              \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_create();                   \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_copy(                       \
      Vector_##TYPE##_##SIZE* other);                                         \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_move(                       \
      Vector_##TYPE##_##SIZE* other);                                         \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_sublist(                    \
      Vector_##TYPE##_##SIZE* other, VECTOR_SIZE_T start, VECTOR_SIZE_T end); \
  void Vector_##TYPE##_##SIZE##_clear(Vector_##TYPE##_##SIZE* vector);        \
  TYPE* Vector_##TYPE##_##SIZE##_get(Vector_##TYPE##_##SIZE* vector,          \
                                     VECTOR_SIZE_T index);                    \
  void Vector_##TYPE##_##SIZE##_set(Vector_##TYPE##_##SIZE* vector,           \
                                    VECTOR_SIZE_T index, TYPE value);         \
  void Vector_##TYPE##_##SIZE##_erase(Vector_##TYPE##_##SIZE* vector,         \
                                      VECTOR_SIZE_T index);                   \
  void Vector_##TYPE##_##SIZE##_push_back(Vector_##TYPE##_##SIZE* vector,     \
                                          TYPE value);

#define DEFINE_VECTOR(TYPE, SIZE)                                              \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_create() {                   \
    Vector_##TYPE##_##SIZE vector;                                             \
    vector.size = 0;                                                           \
    vector.capacity = SIZE;                                                    \
    return vector;                                                             \
  }                                                                            \
                                                                               \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_copy(                        \
      Vector_##TYPE##_##SIZE* other) {                                         \
    Vector_##TYPE##_##SIZE vector;                                             \
    vector.size = other->size;                                                 \
    vector.capacity = SIZE;                                                    \
    memcpy(vector.data, other->data, sizeof(TYPE) * vector.size);              \
    return vector;                                                             \
  }                                                                            \
                                                                               \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_move(                        \
      Vector_##TYPE##_##SIZE* other) {                                         \
    Vector_##TYPE##_##SIZE vector;                                             \
    vector.size = other->size;                                                 \
    vector.capacity = SIZE;                                                    \
    memcpy(vector.data, other->data, sizeof(TYPE) * vector.size);              \
    other->size = 0;                                                           \
    other->capacity = SIZE;                                                    \
    return vector;                                                             \
  }                                                                            \
                                                                               \
  Vector_##TYPE##_##SIZE Vector_##TYPE##_##SIZE##_sublist(                     \
      Vector_##TYPE##_##SIZE* other, VECTOR_SIZE_T start, VECTOR_SIZE_T end) { \
    Vector_##TYPE##_##SIZE vector;                                             \
    vector.size = end - start;                                                 \
    vector.capacity = SIZE;                                                    \
    memcpy(vector.data, other->data + start, sizeof(TYPE) * vector.size);      \
    return vector;                                                             \
  }                                                                            \
                                                                               \
  void Vector_##TYPE##_##SIZE##_clear(Vector_##TYPE##_##SIZE* vector) {        \
    vector->size = 0;                                                          \
  }                                                                            \
                                                                               \
  TYPE* Vector_##TYPE##_##SIZE##_get(Vector_##TYPE##_##SIZE* vector,           \
                                     VECTOR_SIZE_T index) {                    \
    return vector->data + index;                                               \
  }                                                                            \
                                                                               \
  void Vector_##TYPE##_##SIZE##_set(Vector_##TYPE##_##SIZE* vector,            \
                                    VECTOR_SIZE_T index, TYPE value) {         \
    vector->data[index] = value;                                               \
  }                                                                            \
                                                                               \
  void Vector_##TYPE##_##SIZE##_erase(Vector_##TYPE##_##SIZE* vector,          \
                                      VECTOR_SIZE_T index) {                   \
    memmove(vector->data + index, vector->data + index + 1,                    \
            sizeof(TYPE) * (vector->size - index - 1));                        \
    vector->size--;                                                            \
  }                                                                            \
                                                                               \
  void Vector_##TYPE##_##SIZE##_push_back(Vector_##TYPE##_##SIZE* vector,      \
                                          TYPE value) {                        \
    vector->data[vector->size] = value;                                        \
    vector->size++;                                                            \
  }

#define DECLARE_VECTORS(TYPE) \
  DECLARE_VECTOR(TYPE, 4)     \
  DECLARE_VECTOR(TYPE, 8)     \
  DECLARE_VECTOR(TYPE, 16)    \
  DECLARE_VECTOR(TYPE, 32)    \
  DECLARE_VECTOR(TYPE, 64)

#define DEFINE_VECTORS(TYPE) \
  DEFINE_VECTOR(TYPE, 4)     \
  DEFINE_VECTOR(TYPE, 8)     \
  DEFINE_VECTOR(TYPE, 16)    \
  DEFINE_VECTOR(TYPE, 32)    \
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