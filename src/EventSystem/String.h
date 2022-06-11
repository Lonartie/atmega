#ifndef VECTOR_STRING_H
#define VECTOR_STRING_H

#include "../Misc/Vector.h"
#include <stdbool.h>

typedef const char* String;
DECLARE_VECTOR(String);

bool String_equals(String a, String b);
String String_copy(String a);

#endif // VECTOR_STRING_H