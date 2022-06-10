#ifndef F_CPU
# define F_CPU 16000000
#endif

#ifndef __AVR_ATmega328P__
#	define __AVR_ATmega328P__
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <util/delay.h>

#define UNPACK(...) __VA_ARGS__

#define calln(O, F, ARGS) (O).F(&O, UNPACK ARGS)
#define callpn(O, F, ARGS) (O)->F(O, UNPACK ARGS)

#define call(O, F) (O).F(&O)
#define callp(O, F) (O)->F(O)
