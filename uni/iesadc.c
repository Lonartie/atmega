#include <avr/io.h>
#include <stdio.h>
#include "iesusart.h"

// WTF. Why is there no header here? Always seperate declaration,
// documentation, and implementation! Clean this up if you use this!

#define DR_ADC0 DDRC
#define DP_ADC0 DDC0
#define IR_ADC0 PC0
#define IP_ADC0 PC0

#define DR_ADC1 DDRC
#define DP_ADC1 DDC1
#define IR_ADC1 PC1
#define IP_ADC1 PC1

#define DR_ADC2 DDRC
#define DP_ADC2 DDC2
#define IR_ADC2 PC2
#define IP_ADC2 PC2

// The following constants reflect table 23-4 of the ATMega328p-data-
// sheet. These constants
#define ADMUX_CHN_ADC0 0 // 0000 0000
#define ADMUX_CHN_ADC1 1 // 0000 0001
#define ADMUX_CHN_ADC2 2 // 0000 0010
#define ADMUX_CHN_ADC3 3 // 0000 0011
#define ADMUX_CHN_ALL 7 // 0000 1111

// Average some measurements to reduce probable noise.
#define ADC_AVG_WINDOW 20

// Well hmmm understand this by yourself. Become inspired.
#define ADCMSG "ADC0: %5u\tADC1: %5u\tADC2: %5u\n"

/** Initializes the ADC-unit. There is ONE single ADC unit on the
 * microcontroller, but different "channels" (input pins) can be
 * connected to the ADC via a MUX-register. The s4etup of the ADC is
 * done in this method, the MUX is used in the read-function.
 */
void ADC_init(void) {
  // The following lines still let the digital input registers enabled,
  // though that's not a good idea (energy-consumption).

  // Klare Verhältnisse erstma!
  ADMUX = 0;

  // Sets AVcc as the ADC's voltage source and as the reference,
  // while that voltage stems from the AREF-pin (5V when the robots is
  //  powered by batteries, lower otherwise).
  ADMUX = (1<<REFS0);

  // This sets how fast (and: accurate) the ADC can run.
  // All these bits set to 1 set a sysclock-division of 128,
  // so the ADC can run with up to 125 kHz.
  ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

  // This enables/really turns on the ADC. 
  ADCSRA |= (1<<ADEN);

  // The following lines of code start a single measurement in single
  // conversion mode. Needed once to "warm up" the ADC. The contents
  // of the result-register ADCW are ignored because they are not
  // reliable yet (until warm-up is done). ADSC goes to 0 when the
  // conversion is done. The first conversion is not only inreliable,
  // but also 25 ADC-cycles long, while the next conversions are around
  // 13 cycles long.
  ADCSRA |= (1<<ADSC);
  while (ADCSRA & (1<<ADSC)) {
    // zzzZZZzzzZZZzzz ... take a sleep until measurement done.
  }
  ADCW;
}

/** We have a 10-bit-ADC, so somewhere in memory we have to read that
 * 10 bits.  Due to this, this function returns a 16-bit-value.
 */
uint16_t ADC_read(uint8_t channel) {
  // Remember to have the ADC initialized!

  // The following line does set all ADMUX-MUX-pins to 0, disconnects
  // all channels from the MUX.
  ADMUX &= ~(ADMUX_CHN_ALL);
  ADMUX |= channel;

  // We start a single measurement and then busy-wait until
  // the ADSC-bit goes to 0, signalling the end of the measurement.
  ADCSRA |= (1<<ADSC);
  while (ADCSRA & (1<<ADSC)) {
    // zzzZZZzzzZZZzzz ... take a sleep until measurement done.
  }
  // Again, a pointer-airthmetical expression. the ADC-register has a
  // lower and a higher portion, but
  return ADCW;
}

/** Used to read multiple measurements to reduce noise.
 */
uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples) {
  // How large does our datatype need to be?
  uint32_t sum = 0;

  for (uint8_t i = 0; i < nsamples; ++i ) {
    sum += ADC_read(channel);
  }

  return (uint16_t)( sum / (float)nsamples );
}

int main(void) {
    USART_init(UBRR_SETTING);

    DR_ADC0 &= ~(1 << DP_ADC0);
    DR_ADC1 &= ~(1 << DP_ADC1);
    DR_ADC2 &= ~(1 << DP_ADC2);

    ADC_init();

    unsigned char strbuff[sizeof(ADCMSG) + 15]; // WTF, why + 15? Oo

    // Just to make things clear: You have to be extremely careful with
    // the size of the stringbuffer. Better safe than sorry! But memory
    // as well as time are so so so precious!

    uint16_t adcval0 = 0;
    uint16_t adcval1 = 0;
    uint16_t adcval2 = 0;

    while(1) {
        adcval0 = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);
        adcval1 = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
        adcval2 = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);

        sprintf(strbuff, ADCMSG, adcval0, adcval1, adcval2);

        USART_print(strbuff);
    }

    return 0;
}
