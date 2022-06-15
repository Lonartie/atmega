#ifndef VECTOR_STRING_H
#define VECTOR_STRING_H

#include "../Misc/Vector.h"
#include <stdbool.h>

typedef const char* String;
DECLARE_VECTORS(String);

/// @returns true if the strings are equal
bool String_equals(String a, String b);

/// @brief the buffer size for quick formatting of strings
#define FMT_SIZE 64

/// @brief quickly formats a string with a given buffer size of FMT_SIZE
#define FMT(format, ...) ((snprintf(__buffer__, 64, format, __VA_ARGS__ ), __buffer__))

/// @brief the internal buffer to use for quick formatting of strings
extern char __buffer__[FMT_SIZE];

#endif // VECTOR_STRING_H