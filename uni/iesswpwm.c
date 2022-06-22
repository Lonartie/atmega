#include <avr/io.h>
#include <util/delay.h>

typedef enum DIR {UP, DOWN} dir_t;

void waitMsDynamic(unsigned char ms) {
    while(ms--) {
        _delay_ms(1);
    }
}

void waitUsDynamic(unsigned char ms) {
    while(ms--) {
        _delay_us(1);
    }
}

void turnAroundSignal() {
	for (unsigned char i = 0; i < 10; i++) {
            PORTB = (1 << PB5);
            waitMsDynamic(50);
            PORTB &= ~(1 << PB5);
            waitMsDynamic(50);
	}
}

void main() {
    DDRB = (1 << DDB5);
    
    unsigned char T = 255;
    unsigned char duty = 0;
    unsigned char nonDuty = T - duty;
    dir_t dutydir = UP;
    
    while (1) {
        
        nonDuty = T - duty;
        
        for (unsigned char i = 0; i < T/15; i++) {
            PORTB = (1 << PB5);
            waitUsDynamic(duty);
            PORTB &= ~(1 << PB5);
            waitUsDynamic(nonDuty);
        }
        
        if (dutydir == UP) {
			duty++;
		} else {
			duty--;
		}
		
        if (duty == T) {
            dutydir = DOWN;
            waitMsDynamic(150);
            turnAroundSignal();
        } else if (duty == 0) {
			dutydir = UP;
			waitMsDynamic(150);
			turnAroundSignal();
		}
		
    }
}
