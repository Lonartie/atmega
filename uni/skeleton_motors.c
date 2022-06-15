#include "iesmotors.h"
#include <util/delay.h>

int main(void) {
    // Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;

    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);

    // Set PD7 as output (IN1)
    DDRD |= (1 << DD7);

    // Make PWM work on PD[5|6]
    setupTimer0();

    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3);

    // Set the duty cycles for PD5/PD6
    setDutyCycle(PD5, 155);
    setDutyCycle(PD6, 155);

    // Set IN1 to HIGH and don't set IN2 to HIGH (leave LOW) -> Left motors FORWARD
    PORTD |= (1 << PD7); // Use OR, since overwriting will disable EN[A|B]!

    _delay_ms(1000);

    // Set IN1 to LOW and don't set IN2 to HIGH (leave LOW) -> Left motors OFF
    PORTD &= ~(1 << PD7);

    _delay_ms(1000);

    // Set IN2 to HIGH and don't set anything else to HIGH -> Left motors BACKWARD
    PORTB |= (1 << PB0);

    _delay_ms(1000);

    // Set IN2 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PB0);

    _delay_ms(1000);

    // Set IN3 to HIGH and don't set anything else to HIGH -> Right motors BACKWARD
    PORTB |= (1 << PB1);

    _delay_ms(1000);

    // Set IN3 to LOW and don't set anything else to HIGH -> Right motors OFF
    PORTB &= ~(1 << PB1);

    _delay_ms(1000);

    // Set IN4 to HIGH and don't set anything else to HIGH -> Right motors FORWARD
    PORTB |= (1 << PB3);

    _delay_ms(1000);

    // Set IN4 to LOW, nothing else HIGH -> No motors turn
    PORTB &= ~(1 << PB3);

    _delay_ms(1000);

    // Both sides forward
    PORTD |= (1 << PD7);
    PORTB |= (1 << PB3);

    _delay_ms(1000);

    // Both sides stop
    PORTD &= ~(1 << PD7);
    PORTB &= ~(1 << PB3);

    _delay_ms(1000);

    // Both sides backward
    PORTB |= (1 << PB0);
    PORTB |= (1 << PB1);

    _delay_ms(1000);

    // Both sides stop
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);

    // Right motors forward, starting with PWM=0% and slowly increas the duty
    setDutyCycle(PD6, 0);
    PORTB |= (1 << PB3);

    for (unsigned char i = 0; i < 255; i++) {
        setDutyCycle(PD6, i+1);
        _delay_ms(50);
    }

    // Slowly decrease the duty
    for (unsigned char i = 255; i >0; i--) {
        setDutyCycle(PD6, i-1);
        _delay_ms(50);
    }

    setDutyCycle(PD6, 0);
    PORTB &= ~(1 << PB3);

    // Left motors forward, starting with PWM=0% and slowly increas the duty
    setDutyCycle(PD5, 0);
    PORTD |= (1 << PB7);

    for (unsigned char i = 0; i < 255; i++) {
        setDutyCycle(PD5, i+1);
        _delay_ms(50);
    }

    // Slowly decrease the duty
    for (unsigned char i = 255; i >0; i--) {
        setDutyCycle(PD5, i-1);
        _delay_ms(50);
    }

    setDutyCycle(PD5, 0);
    PORTD &= ~(1 << PD7);

    return 0;
}
