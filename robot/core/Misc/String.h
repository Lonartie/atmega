#ifndef VECTOR_STRING_H
#define VECTOR_STRING_H

#include <stdbool.h>

#include "../Misc/Vector.h"

typedef const char* String;
typedef char* OwnedString;
DECLARE_VECTORS(String);

/// @returns true if the strings are equal
bool String_equals(String a, String b);

/// @returns true if string b is a substring of string a
bool String_contains(String a, String b);

/// @returns true if trimmed string a is equal to trimmed string b
bool String_equals_trimmed(String a, String b);

/// @brief the buffer size for quick formatting of strings
#define FMT_SIZE 45

/// @brief quickly formats a string with a given buffer size of FMT_SIZE
#define FMT(format, ...) \
  ((snprintf(FMT_buffer, FMT_SIZE, format, __VA_ARGS__), FMT_buffer))

/// @brief the internal buffer to use for quick formatting of strings
extern char FMT_buffer[FMT_SIZE];

#endif  // VECTOR_STRING_H