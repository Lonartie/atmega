#include "String.h"
DEFINE_VECTOR(String);

bool String_equals(String a, String b) 
{
  return strcmp(a, b) == 0;
}

String String_copy(String a)
{
  return strdup(a);
}