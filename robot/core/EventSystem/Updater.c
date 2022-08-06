#include "Updater.h"

DEFINE_VECTORS(Updater);

Updater Updater_create(void* object, void (*update)(void*)) {
  Updater updater;
  updater.object = object;
  updater.update = update;
  return updater;
}