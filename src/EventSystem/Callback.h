#ifndef CALLBACK_H
#define CALLBACK_H

#include "String.h"
#include "../Std/Vector.h"

typedef void(*CallbackFunction)(uint32_t argc, void** argv);
typedef struct {
  CallbackFunction function;
  String event_type;
} Callback;

DECLARE_VECTOR(Callback);

#endif // CALLBACK_H