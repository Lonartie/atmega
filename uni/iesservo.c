#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define POSITION_DELAY 50

void setupTimer2() {
  // WTF is going on here?
  TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // WTF^2
}

int main() {
    setupTimer2(); // Why not Timer 0? Or Timer 1?

    DDRD |= (1 << DDD3); // WTF is D3?

    while (1) {
        for (unsigned char i = 7; i < 37; i+=1) {
            OCR2B = i; // WTF is i and what is it's unit?
            _delay_ms(POSITION_DELAY); // ?? Oo
        }
        // And WTH come these values from?
        for (unsigned char i = 37; i > 7; i-=1) {
            OCR2B = i;
            _delay_ms(POSITION_DELAY);
        }
    }

    return 0;
}

// Bla



















































































































































// Bla
