#include "UltraSoundSensor.h"
#include "Misc/Utils.h"
#include "EventSystem/HardwareTimer.h"
#include "EventSystem/EventSystem.h"
#include "Menu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static Pin echo_pin_inst;
static uint16_t echo_start_us = 0;
static uint16_t echo_duration = 0;
static bool echo_ready_read = true;
static uint8_t event_distance_instance = 255;

uint8_t duration_to_distance(uint16_t duration) 
{
  const float m_per_sec = 343.2f;
  const float cm_per_sec = m_per_sec * 100.0f;
  const float cm_per_us = cm_per_sec / 1000000.0f;
  const float half_cm_per_us = 0.5f * cm_per_us;
  return duration * half_cm_per_us;
}

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

void UltraSoundSensor_set_event(UltraSoundSensor* _this, uint8_t event_distance, String event)
{
  _this->event = event;
  event_distance_instance = event_distance;
  EventSystem_reg_updater(EventSystem_instance(), Updater_create(_this, UltraSoundSensor_update));
}

ISR(PCINT0_vect)
{
  // not yet started
  if (event_distance_instance == 255) 
  {
    return;
  }

  // rising edge
  if (Pin_read(&echo_pin_inst))
  {
    echo_start_us = micros();
  }
  // falling edge
  else
  {
    echo_duration = micros() - echo_start_us;
    echo_ready_read = true;
  }
}

void UltraSoundSensor_trigger(UltraSoundSensor* _this)
{
  echo_duration = 0;
  echo_ready_read = false;
  Pin_write(&_this->trigger, true);
  _delay_us(10);
  Pin_write(&_this->trigger, false);
}

uint8_t UltraSoundSensor_get_distance(UltraSoundSensor* _this)
{

  while (!echo_ready_read);

  UltraSoundSensor_trigger(_this);
  uint32_t start = millis();
  while (echo_duration == 0 && millis() - start < 20);
  return duration_to_distance(echo_duration);
}

void UltraSoundSensor_update(void* obj) {  
  if (echo_ready_read) 
  {
    if (duration_to_distance(echo_duration) <= event_distance_instance) 
    {
      EventSystem_send_event(EventSystem_instance(), Event_create(((UltraSoundSensor*)obj)->event));
    }
    
    UltraSoundSensor_trigger((UltraSoundSensor*)obj);
  }
}