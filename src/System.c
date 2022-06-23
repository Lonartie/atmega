#include "System.h"
#include "Misc/utils.h"
#include "Menu.h"
#include <avr/io.h>

const int SPEED_DRIVE_SLOW = 0;
const int SPEED_DRIVE = 200;
const int SPEED_TURN = 150;
const uint16_t MEASURE_THRESHOLD_LEFT = 330;
const uint16_t MEASURE_THRESHOLD_MID = 400;
const uint16_t MEASURE_THRESHOLD_RIGHT = 330;

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

typedef enum State {
  STATE_LNF,
  STATE_DRV_FW,
  STATE_DRV_FW_ML,
  STATE_DRV_FW_MR,
  STATE_TRN_LEFT,
  STATE_TRN_RIGHT
} State;

static State state = STATE_LNF;
static bool may_log = false;

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
    Pin_create(&MOTOR_RIGHT_BW_DDR, &MOTOR_RIGHT_BW_PORT, MOTOR_RIGHT_BW_DDR_PIN, MOTOR_RIGHT_BW_PORT_PIN));

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

void turn_left(System* atmega);
void turn_right(System* atmega);
void drive_forward(System* atmega);

void System_drive(void* _this) {
  static bool lleft = false, lmid = false, lright = false;
	System* atmega = (System*) _this;

  if (!atmega->started)
    return;

  uint16_t left_measure = ADCPin_read_avg(&atmega->lf_left, 10);
  uint16_t mid_measure = ADCPin_read_avg(&atmega->lf_middle, 10);
  uint16_t right_measure = ADCPin_read_avg(&atmega->lf_right, 10);

	bool left = left_measure > MEASURE_THRESHOLD_LEFT;
	bool mid = mid_measure > MEASURE_THRESHOLD_MID;
	bool right = right_measure > MEASURE_THRESHOLD_RIGHT;

  if (left != lleft || mid != lmid || right != lright) {
    may_log = true;
    ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);
    Menu_log(LOG_INFO, FMT("%d%d%d\n", left, mid, right));
    Menu_log(LOG_DEBUG, FMT("%d %d %d\n", (int) left_measure, (int) mid_measure, (int) right_measure));
  }

  if (mid && left)                    state = STATE_DRV_FW_ML;
  else if (mid && right)              state = STATE_DRV_FW_MR;
  else if (mid)                       state = STATE_DRV_FW;
  else if (left)                      state = STATE_TRN_LEFT;
  else if (right)                     state = STATE_TRN_RIGHT;
  else if (state == STATE_DRV_FW_ML)  state = STATE_TRN_LEFT;
  else if (state == STATE_DRV_FW_MR)  state = STATE_TRN_RIGHT;

  switch (state) {
    case STATE_LNF:       // fall through
    case STATE_DRV_FW:    // fall through
    case STATE_DRV_FW_ML: // fall through
    case STATE_DRV_FW_MR: drive_forward(atmega); break;
    case STATE_TRN_LEFT:  turn_left(atmega); break;
    case STATE_TRN_RIGHT: turn_right(atmega); break;
  }

  lleft = left;
  lmid = mid;
  lright = right;
  may_log = false;
}

void turn_left(System* atmega)
{
  // left sensor -> steer left -> move right forward
  if (may_log) Menu_log(LOG_DEBUG, "tl\n");
  Motor_drive_backward(&atmega->mt_left, SPEED_TURN);
  Motor_drive_forward(&atmega->mt_right, SPEED_TURN);
}

void turn_right(System* atmega)
{
  // right sensor -> steer right -> move left forward
  if (may_log) Menu_log(LOG_DEBUG, "tr\n");
  Motor_drive_forward(&atmega->mt_left, SPEED_TURN);
  Motor_drive_backward(&atmega->mt_right, SPEED_TURN);
}

void drive_forward(System* atmega)
{
  // only mid sensor -> move forward
  if (may_log) Menu_log(LOG_DEBUG, "df\n");
  Motor_drive_forward(&atmega->mt_left, SPEED_DRIVE);
  Motor_drive_forward(&atmega->mt_right, SPEED_DRIVE);
}