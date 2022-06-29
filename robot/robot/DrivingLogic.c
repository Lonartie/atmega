#include "DrivingLogic.h"
#include "Models/System.h"
#include "Models/Menu.h"
#include "EventSystem/HardwareTimer.h"
#include <stdbool.h>

const int SPEED_DRIVE_SLOW = 0;
const int SPEED_DRIVE = 200;
const int SPEED_TURN = 150;

const uint16_t MEASURE_THRESHOLD_LEFT = 330;
const uint16_t MEASURE_THRESHOLD_MID = 400;
const uint16_t MEASURE_THRESHOLD_RIGHT = 330;

const char* TURN_LEFT_MESSAGE = "tl\n";
const char* TURN_RIGHT_MESSAGE = "tr\n";
const char* DRIVE_FORWARD_MESSAGE = "df\n";
const char* TIMER_MESSAGE = "%ums\n";
const char* SENSORS_MESSAGE = "%d%d%d\n";
const char* SENSORS_DEBUG_MESSAGE = "%d %d %d\n";
const char* US_SENSOR_MESSAGE = "dist: %d\n";

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

void turn_left(System* atmega);
void turn_right(System* atmega);
void drive_forward(System* atmega);

void System_drive(void* _this) {
  static bool lleft = false, lmid = false, lright = false;
	System* atmega = (System*) _this;
  static uint32_t last_time = 0;

  if (!atmega->started)
    return;

  uint16_t left_measure = ADCPin_read_avg(&atmega->lf_left, 10);
  uint16_t mid_measure = ADCPin_read_avg(&atmega->lf_middle, 10);
  uint16_t right_measure = ADCPin_read_avg(&atmega->lf_right, 10);

	bool left = left_measure > MEASURE_THRESHOLD_LEFT;
	bool mid = mid_measure > MEASURE_THRESHOLD_MID;
	bool right = right_measure > MEASURE_THRESHOLD_RIGHT;
  uint8_t us_distance = UltraSoundSensor_get_distance(&atmega->us);

  if (left != lleft || mid != lmid || right != lright) {
    // update lights and sends log messages
    may_log = true;
    ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);
    uint32_t new_time = millis();
    uint32_t time_diff = new_time - last_time;
    last_time = new_time;
    Menu_log(LOG_DEBUG, FMT(TIMER_MESSAGE, time_diff));
    Menu_log(LOG_INFO, FMT(SENSORS_MESSAGE, left, mid, right));
    Menu_log(LOG_DEBUG, FMT(US_SENSOR_MESSAGE, (int) us_distance));
    Menu_log(LOG_DEBUG, FMT(SENSORS_DEBUG_MESSAGE, (int) left_measure, (int) mid_measure, (int) right_measure));
  }

  // there are rare cases where 011 -> 010 -> 000 is detected so 
  // we need to memorize the x0x side and turn the car afterwards
  // so it doesn't drive away from the black line 
  State memorized_state = 
    state == STATE_DRV_FW_ML ? STATE_DRV_FW_ML : 
    state == STATE_DRV_FW_MR ? STATE_DRV_FW_MR : 
                               STATE_DRV_FW; 

  // update state
  if      (mid && left)               state = STATE_DRV_FW_ML;
  else if (mid && right)              state = STATE_DRV_FW_MR;
  else if (mid)                       state = memorized_state;
  else if (left)                      state = STATE_TRN_LEFT;
  else if (right)                     state = STATE_TRN_RIGHT;
  else if (state == STATE_DRV_FW_ML)  state = STATE_TRN_LEFT;
  else if (state == STATE_DRV_FW_MR)  state = STATE_TRN_RIGHT;

  // execute state
  switch (state) {
    case STATE_LNF:       // fall through
    case STATE_DRV_FW:    // fall through
    case STATE_DRV_FW_ML: // fall through
    case STATE_DRV_FW_MR: drive_forward(atmega); break;
    case STATE_TRN_LEFT:  turn_left(atmega); break;
    case STATE_TRN_RIGHT: turn_right(atmega); break;
  }

  lleft    = left;
  lmid     = mid;
  lright   = right;
  may_log  = false;
}

void turn_left(System* atmega)
{
  if (may_log) Menu_log(LOG_DEBUG, TURN_LEFT_MESSAGE);
  Motor_drive_backward(&atmega->mt_left, SPEED_TURN);
  Motor_drive_forward(&atmega->mt_right, SPEED_TURN);
  Servo_set_angle(&atmega->us_servo, -90);
}

void turn_right(System* atmega)
{
  if (may_log) Menu_log(LOG_DEBUG, TURN_RIGHT_MESSAGE);
  Motor_drive_forward(&atmega->mt_left, SPEED_TURN);
  Motor_drive_backward(&atmega->mt_right, SPEED_TURN);
  Servo_set_angle(&atmega->us_servo, 90);
}

void drive_forward(System* atmega)
{
  if (may_log) Menu_log(LOG_DEBUG, DRIVE_FORWARD_MESSAGE);
  Motor_drive_forward(&atmega->mt_left, SPEED_DRIVE);
  Motor_drive_forward(&atmega->mt_right, SPEED_DRIVE);
  Servo_set_angle(&atmega->us_servo, 0);
}