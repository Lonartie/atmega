#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/// @brief ADCPin is a pin that can be used to read an analog value from a pin
typedef struct ADCPin {
  uint8_t channel;
} ADCPin;

/// @brief initializes the ADC in general (for all ADCPins)
void ADC_init();

/// @brief creates a new ADCPin
/// @param channel the channel that the pin is connected to
/// @param ddr the DDR register for the pin
/// @param ddr_pin the DDR bit for the pin
/// @returns a new ADCPin
ADCPin ADCPin_create(uint8_t channel, volatile uint8_t* ddr, uint8_t ddr_pin);

/// @brief reads the value from the \a adc
uint16_t ADCPin_read(ADCPin* adc);

/// @brief reads the value from the \a adc \a samples times and returns the
/// average
uint16_t ADCPin_read_avg(ADCPin* adc, uint8_t samples);

#define ADC_CHANNEL_0 0    // 0000 0000
#define ADC_CHANNEL_1 1    // 0000 0001
#define ADC_CHANNEL_2 2    // 0000 0010
#define ADC_CHANNEL_3 3    // 0000 0011
#define ADC_CHANNEL_ALL 7  // 0000 1111

#endif  // ADC_H