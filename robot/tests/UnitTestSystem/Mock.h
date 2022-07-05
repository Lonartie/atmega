#ifndef MOCK_H
#define MOCK_H

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

void begin_session();
void end_session();

void add_kvp_bool(const char* key, bool value);
bool get_kvp_bool(const char* key);

void add_kvp_string(const char* key, const char* value);
const char* get_kvp_string(const char* key);

void add_kvp_int(const char* key, int value);
int get_kvp_int(const char* key);

void add_kvp_float(const char* key, double value);
double get_kvp_float(const char* key);

void add_kvp_void(const char* key, void* value);
void* get_kvp_void(const char* key);

#define BEGIN_SESSION begin_session()
#define END_SESSION end_session()

#define DISABLE_UNUSED_PARAM_WARNING \
  _Pragma("GCC diagnostic push") \
  _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")

#define ENABLE_UNUSED_PARAM_WARNING \
  _Pragma("GCC diagnostic pop")

#define UNPACK(...) __VA_ARGS__
#define MOCK_FUNCTION(RET, NAME, PARAMS_PACK, CALL_PACK)             \
DISABLE_UNUSED_PARAM_WARNING                                         \
RET NAME PARAMS_PACK {                                               \
   add_kvp_int(#NAME "called", get_kvp_int(#NAME "called") + 1);     \
   return *(RET*)(get_kvp_void(#NAME "return"));                     \
}                                                                    \
ENABLE_UNUSED_PARAM_WARNING

#define MOCK_FUNCTION_V(NAME, PARAMS_PACK, CALL_PACK)                \
DISABLE_UNUSED_PARAM_WARNING                                         \
void NAME PARAMS_PACK {                                              \
   add_kvp_int(#NAME "called", get_kvp_int(#NAME "called") + 1);     \
}                                                                    \
ENABLE_UNUSED_PARAM_WARNING

#endif //MOCK_H
