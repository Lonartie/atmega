#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef struct ADCPin
{
  uint8_t channel;
} ADCPin;

void ADC_init();
ADCPin ADCPin_create(uint8_t channel, volatile uint8_t* ddr, uint8_t ddr_pin);
uint16_t ADCPin_read(ADCPin* adc);
uint16_t ADCPin_read_avg(ADCPin* adc, uint8_t samples);

#define ADC_CHANNEL_0 0 // 0000 0000
#define ADC_CHANNEL_1 1 // 0000 0001
#define ADC_CHANNEL_2 2 // 0000 0010
#define ADC_CHANNEL_3 3 // 0000 0011
#define ADC_CHANNEL_ALL 7 // 0000 1111

#endif // ADC_H