#include "ADCPin.h"

#include <avr/io.h>

#include "Misc/Utils.h"

static bool initialized = false;

void ADC_init(void) {
  if (initialized) {
    return;
  }
  initialized = true;
  ADMUX = 0;
  ADMUX = (1 << REFS0);                                 // voltage source
  ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // ADC clock prescaler
  ADCSRA |= (1 << ADEN);                                // ADC enable
  ADCSRA |= (1 << ADSC);                                // start conversion
  while (ADCSRA & (1 << ADSC))
    ;    // ADC warm-up
  ADCW;  // dummy read
}

ADCPin ADCPin_create(uint8_t channel, volatile uint8_t* ddr, uint8_t ddr_pin) {
  ADC_init();
  ADCPin adc;
  adc.channel = channel;
  (*ddr) &= ~(1 << ddr_pin);
  return adc;
}

uint16_t ADCPin_read(ADCPin* adc) {
  ADMUX &= ~(ADC_CHANNEL_ALL);
  ADMUX |= adc->channel;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;  // Wait until done
  return ADCW;
}

uint16_t ADCPin_read_avg(ADCPin* adc, uint8_t nsamples) {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < nsamples; ++i) {
    sum += ADCPin_read(adc);
  }
  return (uint16_t)(sum / (float)nsamples);
}