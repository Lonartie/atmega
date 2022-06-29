#include "UltraSoundSensor.h"
#include "Misc/Utils.h"
#include "EventSystem/HardwareTimer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static Pin echo_pin_inst;
static uint16_t echo_start_us = 0;
static uint16_t echo_duration = 0;

UltraSoundSensor UltraSoundSensor_create(
  Pin trigger, Pin echo, 
  volatile uint8_t* pci_reg, uint8_t pci_group, 
  volatile uint8_t* pci_mask, uint8_t pci_pin)
{
  UltraSoundSensor sensor;
  sensor.trigger = trigger;
  sensor.echo = echo;
  sensor.pci_reg = pci_reg;
  sensor.pci_group = pci_group;
  sensor.pci_mask = pci_mask;
  sensor.pci_pin = pci_pin;

  echo_pin_inst = echo;

  Pin_set_write(&trigger);
  Pin_set_read(&echo);

  // setup pin change interrupt for echo pin
  cli();
  *(sensor.pci_reg) |= (1 << sensor.pci_group);
  *(sensor.pci_mask) |= (1 << sensor.pci_pin);
  sei();

  return sensor;
}

ISR(PCINT0_vect)
{
  // rising edge
  if (Pin_read(&echo_pin_inst))
  {
    echo_start_us = micros();
  }
  // falling edge
  else
  {
    echo_duration = micros() - echo_start_us;
  }
}

uint8_t UltraSoundSensor_get_distance(UltraSoundSensor* _this)
{
  Pin_write(&_this->trigger, true);
  _delay_us(10);
  Pin_write(&_this->trigger, false);
  while (!Pin_read(&_this->echo));
  while (Pin_read(&_this->echo));
  return (uint8_t) (echo_duration / 58.0f);
}