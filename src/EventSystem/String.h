#ifndef VECTOR_STRING_H
#define VECTOR_STRING_H

#include "../Std/Vector.h"
#include <stdbool.h>

typedef const char* String;
DECLARE_VECTOR(String);

bool String_equals(String a, String b);

#endif // VECTOR_STRING_H