#include "String.h"
DEFINE_VECTORS(String);

char __buffer__[FMT_SIZE];

bool String_equals(String a, String b) 
{
  return strcmp(a, b) == 0;
}