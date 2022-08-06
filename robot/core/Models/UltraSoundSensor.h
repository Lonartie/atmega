#ifndef ULTRA_SOUND_SENSOR_H
#define ULTRA_SOUND_SENSOR_H

#include <stdint.h>

#include "Misc/String.h"
#include "Pin.h"

/// @brief represents a ultrasonic sensor
typedef struct UltraSoundSensor {
  Pin trigger;  // pin to trigger the sensor
  Pin echo;     // pin to read the echo from the sensor
  volatile uint8_t* pci_reg;
  uint8_t pci_group;
  volatile uint8_t* pci_mask;
  uint8_t pci_pin;
  String event;  // event to trigger when the sensor is triggered
} UltraSoundSensor;

/// @brief creates a new ultrasonic sensor with the given pins
/// @param trigger the pin to trigger the sensor
/// @param echo the pin to read the echo from the sensor
/// @param pci_reg the register for the pin change interrupt
/// @param pci_group the group for the pin change interrupt
/// @param pci_mask the mask for the pin change interrupt
/// @param pci_pin the pin for the pin change interrupt
/// @returns a new ultrasonic sensor
UltraSoundSensor UltraSoundSensor_create(Pin trigger, Pin echo,
                                         volatile uint8_t* pci_reg,
                                         uint8_t pci_group,
                                         volatile uint8_t* pci_mask,
                                         uint8_t pci_pin);

/// @brief sets the event to send when the sensor is triggered
/// @param _this the ultrasonic sensor to set the event of
/// @param event_distance the distance activation threshold
/// @param event the event to send when the sensor is triggered
void UltraSoundSensor_set_event(UltraSoundSensor* _this, uint8_t event_distance,
                                String event);

/// @brief triggers the sensor
/// @param _this the ultrasonic sensor to trigger
void UltraSoundSensor_trigger(UltraSoundSensor* _this);

/// @brief reads the distance (in cm) from the sensor manually
/// @param _this the ultrasonic sensor to read from
/// @returns the distance (in cm) from the sensor
uint8_t UltraSoundSensor_get_distance(UltraSoundSensor* _this);

/// @brief updates the UltraSoundSensor to check for event trigger
/// @param _this the ultrasonic sensor to update
void UltraSoundSensor_update(void* _this);

/// @brief reads the distance (in cm) from the sensor after the last update
/// @param _this the ultrasonic sensor to read from
/// @returns the distance (in cm) from the sensor
uint16_t UltraSoundSensor_dist(UltraSoundSensor* _this);

#endif  // ULTRA_SOUND_SENSOR_H