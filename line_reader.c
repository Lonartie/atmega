#include <avr/io.h>
#include "iesusart.h"

enum bool_t { false, true }; 
typedef bool_t bool;

int main(void) {
	
    DDRC &= ~((1<<DDC0) | (1<<DDC1) | (1<<DDC2));	// read from PC0-2
    DDRD |= (1 << DDD4);							// write to PD4
    DDRB |= (1 << DDB2);							// write to PB2
	USART_init(UBRR_SETTING);						// init debug port

	while (1) {
		bool clk = (DDRD & (1 << DDD4));
	}		
}
