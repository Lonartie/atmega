#ifndef ULTRA_SOUND_SENSOR_H
#define ULTRA_SOUND_SENSOR_H

#include "Pin.h"
#include <stdint.h>

typedef struct UltraSoundSensor {
  Pin trigger;
  Pin echo;
  volatile uint8_t* pci_reg;
  uint8_t pci_group;
  volatile uint8_t* pci_mask;
  uint8_t pci_pin;
} UltraSoundSensor;

UltraSoundSensor UltraSoundSensor_create(
  Pin trigger, Pin echo, 
  volatile uint8_t* pci_reg, uint8_t pci_group, 
  volatile uint8_t* pci_mask, uint8_t pci_pin
);

uint8_t UltraSoundSensor_get_distance(UltraSoundSensor* _this);

#endif // ULTRA_SOUND_SENSOR_H