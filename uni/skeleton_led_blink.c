#include <avr/io.h>
#define F_CPU 16E6
#include <util/delay.h>


int main(void) {
    // Set Data Direction Register B5 as output.
    DDRB = (1<<DDB5);
    
    while (1) {
        // Set pin/bit 5 to high (VCC) and wait half a sec.
        PORTB = (1<<PORTB5);
        _delay_ms(500);
        
        // Set pin/bit 5 to low (GND) and wait half a sec.
        PORTB &= ~(1<<PORTB5);
        _delay_ms(500);
    }
    return 0;
}
