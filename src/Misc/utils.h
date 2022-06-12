#ifndef F_CPU
# define F_CPU 16000000
#endif

#ifndef __AVR_ATmega328P__
#	define __AVR_ATmega328P__
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Actor.h"

#define MAYBE_UNUSED __attribute__((unused))
