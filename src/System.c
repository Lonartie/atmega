#include "System.h"
#include "Misc/utils.h"
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


System System_create()
{
  System system;
  system.led_strip = ShiftRegister_create(&DDRD, &PORTD, DDD4, PORTD4, &DDRB, &PORTB, DDB2, PORTB2, 3, 1);
  system.logger = USART_create();
  system.lf_left = ADCPin_create(ADC_CHANNEL_2, &DDRC, DDC2);
  system.lf_middle = ADCPin_create(ADC_CHANNEL_1, &DDRC, DDC1);
  system.lf_right = ADCPin_create(ADC_CHANNEL_0, &DDRC, DDC0);
  system.mt_left = Motor_create(PD5, Pin_create(&DDRD, &PORTD, DDD7, PORTD7), Pin_create(&DDRB, &PORTB, DDB0, PORTB0));
  system.mt_right = Motor_create(PD6, Pin_create(&DDRB, &PORTB, DDB3, PORTB3), Pin_create(&DDRB, &PORTB, DDB1, PORTB1));

  ShiftRegister_clear(&system.led_strip);

  return system;
}