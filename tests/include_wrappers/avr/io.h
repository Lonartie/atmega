#ifndef IO_H
#define IO_H

#include <stdint.h>

#define DDRD _ddrd
#define PORTD _portd

#define DDRC _ddrc
#define PORTC _portc

#define DDRB _ddrb
#define PORTB _portb

#define DDB0 _ddb0
#define PORTB0 _portb0

#define DDB1 _ddb1
#define PORTB1 _portb1

#define DDB2 _ddb2
#define PORTB2 _portb2

#define DDB3 _ddb3
#define PORTB3 _portb3

#define DDC0 _ddc0
#define PORTC0 _portc0

#define DDC1 _ddc1
#define PORTC1 _portc1

#define DDC2 _ddc2
#define PORTC2 _portc2

#define DDD4 _ddd4
#define PORTD4 _portd4

#define DDD7 _ddd7
#define PORTD7 _portd7

#define PD5 _pd5
#define PD6 _pd6

#define ADMUX _admux

#define REFS0 _refs0

#define ADCSRA _adcsra

#define ADPS0 _adps0
#define ADPS1 _adps1
#define ADPS2 _adps2

#define ADEN _aden

#define DECLARE_REGISTER(NAME) \
extern volatile uint8_t NAME;

#define DECLARE_PIN(NAME) \
extern volatile uint8_t NAME;

#define DECLARE_REGISTERS(NAME) \
DECLARE_REGISTER(_ddr##NAME) \
DECLARE_REGISTER(_port##NAME)

#define DECLARE_PINS(NAME) \
DECLARE_PIN(_dd##NAME) \
DECLARE_PIN(_port##NAME)

DECLARE_REGISTERS(b)
DECLARE_REGISTERS(c)
DECLARE_REGISTERS(d)

DECLARE_PINS(b0)
DECLARE_PINS(b1)
DECLARE_PINS(b2)
DECLARE_PINS(b3)
DECLARE_PINS(c0)
DECLARE_PINS(c1)
DECLARE_PINS(c2)
DECLARE_PINS(d4)
DECLARE_PINS(d7)

DECLARE_PIN(_pd5)
DECLARE_PIN(_pd6)
DECLARE_PIN(_admux)
DECLARE_PIN(_refs0)
DECLARE_PIN(_adcsra)
DECLARE_PIN(_adps0)
DECLARE_PIN(_adps1)
DECLARE_PIN(_adps2)
DECLARE_PIN(_aden)

#endif // IO_H