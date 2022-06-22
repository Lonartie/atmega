#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "iesusart.h"
#define ECHO PB4
#define TRIGGER PB5

uint16_t cnt = 0;
uint16_t echo_start = 0;
uint16_t echo_end = 0;
int16_t echo_duration = 0;

ISR (PCINT0_vect) {
    if (PINB & (1 << ECHO)) {
        // JUST RISEN
        echo_start = cnt;
    } else {
        // JUST FALLEN
        if (cnt < echo_start) {
            // Need to do something pretty here.
            // This CAN occur.
        }
        echo_duration = cnt - echo_start;
    }
}

ISR (TIMER1_COMPA_vect) {
  cnt+=1;
}

void setupTimer1() {
  cli();
  TCCR1B |= (1 << CS10); // Prescaler: 1 => 16E6 ticks/second
  TCCR1B |= (1 << WGM12); // Use Timer 1 in CTC-mode
  TIMSK1 |= (1 << OCIE1A); // Enable compare-match-interrupt for OCR1A
  OCR1A = 255;           // Every 16E6/255 ticks COMPA_vect is fired.
                         // This equals an (non-existent) 512-clock-divisor.
                         // We need this information for later calculations.
                         // BTW: Keep in mind that there is one more OCR-register
                         // for timer 1, which you can use to do some more neat
                         // stuff.
  sei();
}

void setupPCINTPB4() {
    cli();
    PCICR |= (1<<PCIE0); // Enable Pin Change Interrupt group 0.
    PCMSK0 |= (1<<PCINT4); // Setting the Pin Change interrupt-mask register to reflect
                           // PCINT4's interrupt (pin change) state.
    sei();
}

int main(void) {
    DDRB |= (1 << TRIGGER);
    DDRB &= ~(1 << ECHO);

    USART_init(UBRR_SETTING);

    setupTimer1();

    setupPCINTPB4();

    while(1) {
        PORTB &= ~(1 << TRIGGER);
        _delay_us(10);
        PORTB |= (1 << TRIGGER); // WTF.
        _delay_us(10);
        PCMSK0 |= (1<<PCINT4); // Why? WTF? Why not use cli/sei? For a good reason, for sure.
                               // Think about the timer and cnt.
        cnt = 0;               // WTF? Why cnt=0?
        PCMSK0 &= ~(1<<PCINT4);// Hu?
        PORTB &= ~(1 << TRIGGER);
        _delay_us(2);
        PCMSK0 |= (1<<PCINT4);
        _delay_ms(500); // Hmmmmm. How fast can this whole thing become?

        char strbuff[25];

        USART_print("ECHO DURATION: ");
        sprintf(strbuff, "%u", echo_duration);
        USART_print(strbuff);

        float ticks_per_compmatch = 16E6 / 255.0;
        float of_per_sec = 1 / ticks_per_compmatch;
        float m_per_sec = 343.0;
        float distance = echo_duration * of_per_sec * m_per_sec / 2.0 * 100;
        int idis = (int) distance;

        USART_print("     OBSTACLE DISTANCE: ");
        sprintf(strbuff, "%u DUR: %d CNT: %u\n", idis, echo_duration, cnt);
        USART_print(strbuff);
    }

    return 0;

}


// bla wtf wtf blubb






































































































// bla bli blubb
