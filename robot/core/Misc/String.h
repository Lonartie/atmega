#ifndef VECTOR_STRING_H
#define VECTOR_STRING_H

#include <stdbool.h>

#include "../Misc/Vector.h"

typedef const char* String;
typedef char* OwnedString;
DECLARE_VECTORS(String);

/// @brief compares two strings for equality
/// @param a the first string to compare
/// @param b the second string to compare
/// @returns true if the strings are equal, false otherwise
bool String_equals(String a, String b);

/// @brief checks if b is substring of a
/// @param a the string to check
/// @param b the substring to check
/// @returns true if string b is a substring of string a
bool String_contains(String a, String b);

/// @brief checks for equality of a and b when they are trimmed
/// @param a the first string to compare
/// @param b the second string to compare
/// @returns true if trimmed string a is equal to trimmed string b
bool String_equals_trimmed(String a, String b);

/// @brief the buffer size for quick formatting of strings
#define FMT_SIZE 45

/// @brief quickly formats a string with a given buffer size of FMT_SIZE
#define FMT(FORMAT, ...) \
  ((snprintf(FMT_buffer, FMT_SIZE, FORMAT, __VA_ARGS__), FMT_buffer))

/// @brief the internal buffer to use for quick formatting of strings
extern char FMT_buffer[FMT_SIZE];

#endif  // VECTOR_STRING_H