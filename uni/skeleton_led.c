#include <avr/io.h>

int main(void) {
    // Set Data Direction Register B, Pin 5 as output.
    DDRB = (1<<DDB5);
    // Set pins/bits 5 to high (VCC).
    PORTB = (1<<PORTB5);
    return 0;
}
