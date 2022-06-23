#include "String.h"
DEFINE_VECTORS(String);

char __buffer__[FMT_SIZE];

bool String_equals(String a, String b) 
{
  return strcmp(a, b) == 0;
}

bool String_contains(String a, String b)
{
  return strstr(a, b) != NULL;
}