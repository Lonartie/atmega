#ifndef UPDATER_H
#define UPDATER_H

#include "Vector.h"

typedef struct {
  void* object;
  void (*update)(void* object);
} Updater;

DECLARE_VECTOR(Updater);

Updater Updater_create(void* object, void (*update)(void* object));

#endif // UPDATER_H