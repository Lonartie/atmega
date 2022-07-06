#include "String.h"
DEFINE_VECTORS(String);

char __buffer__[FMT_SIZE];

bool String_equals(String a, String b) { return strcmp(a, b) == 0; }

bool String_contains(String a, String b) { return strstr(a, b) != NULL; }

bool String_equals_trimmed(String a, String b) {
  int la = (int)strlen(a);
  int lb = (int)strlen(b);
  int ia = 0;
  int ib = 0;

  // ignore white spaces at start
  while (ia < la && (a[ia] == ' ' || a[ia] == '\r' || a[ia] == '\n')) ia++;
  while (ib < lb && (b[ib] == ' ' || b[ib] == '\r' || b[ib] == '\n')) ib++;

  // check for equality in the middle
  for (; ia < la && ib < lb; ia++, ib++)
    if (a[ia] != b[ib]) return false;

  // ignore white spaces at end
  while (ia < la && (a[ia] == ' ' || a[ia] == '\r' || a[ia] == '\n')) ia++;
  while (ib < lb && (b[ib] == ' ' || b[ib] == '\r' || b[ib] == '\n')) ib++;

  // check both are at the end
  return ia == la && ib == lb;
}