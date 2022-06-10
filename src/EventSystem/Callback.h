#ifndef CALLBACK_H
#define CALLBACK_H

#include "../Std/Vector.h"

typedef void(*Callback)(uint32_t argc, void** argv);
DECLARE_VECTOR(Callback);
DECLARE_VECTOR(Vector_Callback);

#endif // CALLBACK_H