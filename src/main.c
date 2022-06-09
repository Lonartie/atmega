#include "utils.h"

typedef enum bool
{
	false,
	true
} bool;

typedef enum Receiver
{
	Left, Mid, Right
} Receiver;

void init									();
void writeRegisterValues	(bool left, bool mid, bool right);
bool readReceiver					(Receiver receiver);

int main(void)
{
	init();
	bool direction = true;
	int light = 0;
	while (1)
	{
		_delay_ms(500);

		if (light == 0) writeRegisterValues(true, false, false);
		if (light == 1) writeRegisterValues(false, true, false);
		if (light == 2) writeRegisterValues(false, false, true);
		
		if (light == 2 || light == 0)
			direction = !direction;

		if (direction) 	light++;
		else 						light--; 
	}
}

void init() 
{
	DDRC &= ~((1 << DDC0)			// read from PC0 (LEFT REC)
						| (1 << DDC1)		// read from PC1 (MID REC)
						| (1 << DDC2)); // read from PC2 (RIGHT REC)
	DDRD |= (1 << DDD4);			// write to PD4 (CLOCK)
	DDRB |= (1 << DDB2);			// write to PB2 (DATA)
	USART_init(UBRR_SETTING); // init debug port
}

void writeClock 				(bool value);
void writeRegisterValue	(bool value);

void writeRegisterValues(bool left, bool mid, bool right)
{
	writeClock(0); _delay_ms(1); writeRegisterValue(right); writeClock(1); _delay_ms(1);
	writeClock(0); _delay_ms(1); writeRegisterValue(mid); 	writeClock(1); _delay_ms(1);
	writeClock(0); _delay_ms(1); writeRegisterValue(left); 	writeClock(1); _delay_ms(1);
	writeClock(0); _delay_ms(1);
}

void writeClock(bool value) 
{
	if (value) 	PORTD |=  (1 << DDD4);
	else				PORTD &= ~(1 << DDD4);
}

void writeRegisterValue(bool value)
{
	if (value) 	PORTB |=  (1 << DDB2);
	else				PORTB &= ~(1 << DDB2);
}

bool readReceiver(Receiver receiver)
{
	if (receiver == Left) 	return PORTC & (1 << DDC0);
	if (receiver == Mid)		return PORTC & (1 << DDC1);
	if (receiver == Right)		return PORTC & (1 << DDC2); 	
	return 0;
}