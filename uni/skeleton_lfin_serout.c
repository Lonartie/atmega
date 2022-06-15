#include <avr/io.h>
#include "iesusart.h"

int main(void) {
	
    // Set Data Direction Register C [0|1|2| as input.
    DDRC = ~((1<<DDC0) | (1<<DDC1) | (1<<DDC2));
    
    // Initialze U(S)ART!
    USART_init(UBRR_SETTING);
    
    while (1) {
		
        if (PINC & (1 << PINC0)) {
            USART_print("BLA\n");
        }
        if (PINC & (1 << PINC1)) {
            USART_print("BLI\n");
        }
        if (PINC & (1 << PINC2)) {
            USART_print("BLUBB\n");
        }
        
    }
    
    return 0;
    
}
