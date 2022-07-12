#ifndef ULTRA_SOUND_SENSOR_H
#define ULTRA_SOUND_SENSOR_H

#include <stdint.h>

#include "Misc/String.h"
#include "Pin.h"

typedef struct UltraSoundSensor {
  Pin trigger;
  Pin echo;
  volatile uint8_t* pci_reg;
  uint8_t pci_group;
  volatile uint8_t* pci_mask;
  uint8_t pci_pin;
  String event;
} UltraSoundSensor;

UltraSoundSensor UltraSoundSensor_create(Pin trigger, Pin echo,
                                         volatile uint8_t* pci_reg,
                                         uint8_t pci_group,
                                         volatile uint8_t* pci_mask,
                                         uint8_t pci_pin);

void UltraSoundSensor_set_event(UltraSoundSensor* _this, uint8_t event_distance,
                                String event);

void UltraSoundSensor_trigger(UltraSoundSensor* _this);

uint8_t UltraSoundSensor_get_distance(UltraSoundSensor* _this);

/// @brief updates the UltraSoundSensor to check for event trigger
void UltraSoundSensor_update(void* obj);

uint16_t UltraSoundSensor_dist(UltraSoundSensor* _this);

#endif  // ULTRA_SOUND_SENSOR_H