#include "System.h"
#include "Misc/Utils.h"
#include "EventSystem/HardwareTimer.h"
#include <avr/io.h>

#define LED_CLK_DDR         DDRD
#define LED_CLK_PORT        PORTD
#define LED_CLK_DDR_PIN     DDD4
#define LED_CLK_PORT_PIN    PORTD4

#define LED_DATA_DDR        DDRB
#define LED_DATA_PORT       PORTB
#define LED_DATA_DDR_PIN    DDB2
#define LED_DATA_PORT_PIN   PORTB2

#define LF_LEFT_DDR         DDRC
#define LF_LEFT_DDR_PIN     DDC2
#define LF_MID_DDR          DDRC
#define LF_MID_DDR_PIN      DDC1
#define LF_RIGHT_DDR        DDRC
#define LF_RIGHT_DDR_PIN    DDC0

#define MOTOR_LEFT_PWM_PIN        PD5
#define MOTOR_LEFT_FW_DDR         DDRD
#define MOTOR_LEFT_FW_PORT        PORTD
#define MOTOR_LEFT_FW_DDR_PIN     DDD7
#define MOTOR_LEFT_FW_PORT_PIN    PORTD7
#define MOTOR_LEFT_BW_DDR         DDRB
#define MOTOR_LEFT_BW_PORT        PORTB
#define MOTOR_LEFT_BW_DDR_PIN     DDB0
#define MOTOR_LEFT_BW_PORT_PIN    PORTB0

#define MOTOR_RIGHT_PWM_PIN        PD6
#define MOTOR_RIGHT_FW_DDR         DDRB
#define MOTOR_RIGHT_FW_PORT        PORTB
#define MOTOR_RIGHT_FW_DDR_PIN     DDB3
#define MOTOR_RIGHT_FW_PORT_PIN    PORTB3
#define MOTOR_RIGHT_BW_DDR         DDRB
#define MOTOR_RIGHT_BW_PORT        PORTB
#define MOTOR_RIGHT_BW_DDR_PIN     DDB1
#define MOTOR_RIGHT_BW_PORT_PIN    PORTB1

#define US_TRIGGER_DDR             DDRB
#define US_TRIGGER_PORT            PORTB
#define US_TRIGGER_DDR_PIN         DDB5
#define US_TRIGGER_PORT_PIN        PORTB5

#define US_ECHO_DDR                DDRB
#define US_ECHO_PORT               PINB
#define US_ECHO_DDR_PIN            DDB4
#define US_ECHO_PORT_PIN           PORTB4

#define US_PCI_REG                 PCICR
#define US_PCI_GROUP               PCIE0
#define US_PCI_MASK                PCMSK0
#define US_PCI_PIN                 PCINT4

#define US_SERVO_DDR               DDRD
#define US_SERVO_PORT              PORTD
#define US_SERVO_DDR_PIN           DDD3
#define US_SERVO_PORT_PIN          PORTD3
#define US_SERVO_OCR               OCR2B
#define US_SERVO_DELAY_MS          0

System System_create()
{
  System system;

  system.led_strip = ShiftRegister_create(
    &LED_CLK_DDR, &LED_CLK_PORT, LED_CLK_DDR_PIN, LED_CLK_PORT_PIN, 
    &LED_DATA_DDR, &LED_DATA_PORT, LED_DATA_DDR_PIN, LED_DATA_PORT_PIN, 
    3, 1
  );

  system.lf_left = ADCPin_create(ADC_CHANNEL_2, &LF_LEFT_DDR, LF_LEFT_DDR_PIN);
  system.lf_middle = ADCPin_create(ADC_CHANNEL_1, &LF_MID_DDR, LF_MID_DDR_PIN);
  system.lf_right = ADCPin_create(ADC_CHANNEL_0, &LF_RIGHT_DDR, LF_RIGHT_DDR_PIN);

  system.mt_left = Motor_create(MOTOR_LEFT_PWM_PIN, 
    Pin_create(&MOTOR_LEFT_FW_DDR, &MOTOR_LEFT_FW_PORT, MOTOR_LEFT_FW_DDR_PIN, MOTOR_LEFT_FW_PORT_PIN), 
    Pin_create(&MOTOR_LEFT_BW_DDR, &MOTOR_LEFT_BW_PORT, MOTOR_LEFT_FW_DDR_PIN, MOTOR_LEFT_BW_PORT_PIN)
  );

  system.mt_right = Motor_create(MOTOR_RIGHT_PWM_PIN, 
    Pin_create(&MOTOR_RIGHT_FW_DDR, &MOTOR_RIGHT_FW_PORT, MOTOR_RIGHT_FW_DDR_PIN, MOTOR_RIGHT_FW_PORT_PIN), 
    Pin_create(&MOTOR_RIGHT_BW_DDR, &MOTOR_RIGHT_BW_PORT, MOTOR_RIGHT_BW_DDR_PIN, MOTOR_RIGHT_BW_PORT_PIN)
  );

  system.us = UltraSoundSensor_create(
    Pin_create(&US_TRIGGER_DDR, &US_TRIGGER_PORT, US_TRIGGER_DDR_PIN, US_TRIGGER_PORT_PIN), 
    Pin_create(&US_ECHO_DDR, &US_ECHO_PORT, US_ECHO_DDR_PIN, US_ECHO_PORT_PIN),
    &US_PCI_REG, US_PCI_GROUP, &US_PCI_MASK, US_PCI_PIN
  );

  system.us_servo = Servo_create(
    Pin_create(&US_SERVO_DDR, &US_SERVO_PORT, US_SERVO_DDR_PIN, US_SERVO_PORT_PIN),
    &US_SERVO_OCR, US_SERVO_DELAY_MS
  );

  system.started = false;

  ShiftRegister_clear(&system.led_strip);

  return system;
}

void System_start(void* _this) {
  ((System*)_this)->started = true;
}

void System_stop(void* _this) {
  ((System*)_this)->started = false;
  Motor_stop(&((System*)_this)->mt_left);
  Motor_stop(&((System*)_this)->mt_right);
}

