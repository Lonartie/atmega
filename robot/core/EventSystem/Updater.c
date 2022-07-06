#include "Updater.h"

DEFINE_VECTORS(Updater);

Updater Updater_create(void* object, void (*update)(void* object)) {
  Updater updater;
  updater.object = object;
  updater.update = update;
  return updater;
}