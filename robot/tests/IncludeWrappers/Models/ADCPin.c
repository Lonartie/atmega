#include "Models/ADCPin.h"
#include "UnitTestSystem/Mock.h"

MOCK_FUNCTION_V(ADC_init, (),());
MOCK_FUNCTION(ADCPin, ADCPin_create, (uint8_t channel, volatile uint8_t* ddr, uint8_t ddr_pin), (channel, ddr, ddr_pin));
MOCK_FUNCTION(uint16_t, ADCPin_read, (ADCPin* adc), (adc));
MOCK_FUNCTION(uint16_t, ADCPin_read_avg, (ADCPin* adc, uint8_t samples), (adc, samples));