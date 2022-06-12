#ifndef UPDATER_H
#define UPDATER_H

#include "../Misc/Vector.h"

typedef struct Updater {
  void* object;
  void (*update)(void* object);
} Updater;

DECLARE_VECTORS(Updater);

Updater Updater_create(void* object, void (*update)(void* object));

#endif // UPDATER_H