#include <avr/io.h>
#include "iesusart.h"

int main(void) {
    USART_init(UBRR_SETTING);
    USART_print("HALLO WELT!\n");
    return 0;
}
