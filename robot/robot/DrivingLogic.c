#include "DrivingLogic.h"

#include <stdbool.h>
#include <util/delay.h>

#include "EventSystem/EventSystem.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/Menu.h"
#include "Models/System.h"
#include "Models/WatchDog.h"

const int SPEED_DRIVE_SLOW = 0;
const int SPEED_DRIVE = 200;
const int SPEED_TURN = 180;

uint8_t US_SENSOR_DISTANCE = 15;

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

static bool wall_detected = false;

void drive_logic(System* atmega);
void turn_left(System* atmega, bool may_log);
void turn_right(System* atmega, bool may_log);
void drive_forward(System* atmega, bool may_log);

void detect_wall(void* system) {
  System* atmega = (System*)system;

  wall_detected = UltraSoundSensor_dist(&atmega->us) <= US_SENSOR_DISTANCE;

  // static uint32_t last_detected = 0;
  /*if (micros() - last_detected < 1000000 &&
      UltraSoundSensor_dist(&atmega->us) <= US_SENSOR_DISTANCE) {
    last_detected = micros();
    wall_detected = true;
  } else {
    wall_detected = false;
  }*/

  /*if (UltraSoundSensor_dist(&atmega->us) > 20) {
    return;
  }

  static uint8_t walls_per_second = 0;
  static uint32_t last_wps_t = 0;
  walls_per_second++;

  if (micros() - last_wps_t > 50000) {
    wall_detected = (walls_per_second > 2);
    last_wps_t = micros();
    walls_per_second = 0;
  }*/
}

void Logic_restart(void* system) {
  System* atmega = (System*)system;
  Servo_set_angle(&atmega->us_servo, 0);
}

void Logic_start(void* system) {
  System* atmega = (System*)system;
  Servo_set_angle(&atmega->us_servo, 0);

  UltraSoundSensor_set_event(&atmega->us, US_SENSOR_DISTANCE, "US_SENSOR");
  EventSystem_reg_listener(EventSystem_instance(),
                           Listener_create_r(system, detect_wall, "US_SENSOR"));
}

void Logic_drive_infinite(void* system) {
  System* atmega = (System*)system;
  drive_logic(atmega);
}

void Logic_drive_3_rounds(void* system MAYBE_UNUSED) {}

void drive_logic(System* atmega) {
  static State state = STATE_LNF;
  static bool lleft = false, lmid = false, lright = false;
  static uint32_t last_time = 0;
  static bool may_see_start = true;
  static bool seeing_start = false;
  static uint64_t time_seeing_start = 0;
  static uint8_t rounds = 0;

  if (!atmega->started) return;

  uint16_t left_measure = ADCPin_read_avg(&atmega->lf_left, 10);
  uint16_t mid_measure = ADCPin_read_avg(&atmega->lf_middle, 10);
  uint16_t right_measure = ADCPin_read_avg(&atmega->lf_right, 10);

  bool left = left_measure > MEASURE_THRESHOLD_LEFT;
  bool mid = mid_measure > MEASURE_THRESHOLD_MID;
  bool right = right_measure > MEASURE_THRESHOLD_RIGHT;

  bool may_log = false;
  uint32_t new_time = micros();

  if (left != lleft || mid != lmid || right != lright) {
    // update lights and sends log messages
    may_log = true;
    ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);
    new_time = micros();
    uint32_t time_diff = (new_time - last_time) / 1000;
    last_time = new_time;
    Menu_log(LOG_DEBUG, FMT(TIMER_MESSAGE, time_diff));
    Menu_log(LOG_INFO, FMT(SENSORS_MESSAGE, left, mid, right));
    Menu_log(LOG_DEBUG, FMT(SENSORS_DEBUG_MESSAGE, (int)left_measure,
                            (int)mid_measure, (int)right_measure));
  }

  static uint8_t wall_phase = 0;
  static uint32_t init_t = 0;

  if (wall_phase != 0 && mid && micros() - init_t >= 3000000) {
    Menu_log(LOG_INFO, "found track again\n");
    Servo_set_angle(&atmega->us_servo, 0);
    _delay_ms(1000);
    turn_right(atmega, may_log);
    wall_phase = 0;
    wall_detected = false;
    US_SENSOR_DISTANCE = 15;
  }

  if (wall_detected && wall_phase == 0) {
    // setup phase
    init_t = micros();
    Menu_log(LOG_INFO, "phase 0 -> 1\n");
    Servo_set_angle(&atmega->us_servo, -90);
    wall_phase = 1;
    US_SENSOR_DISTANCE = 20;
    return;
  } else if (wall_phase == 1) {
    // turn right phase
    Menu_log(LOG_INFO, "phase 1 tr\n");
    turn_right(atmega, may_log);
    _delay_ms(100);

    if (wall_detected) {
      wall_phase = 2;
    }
    return;
  } else if (wall_phase == 2) {
    // drive forward phase
    Menu_log(LOG_INFO, "phase 2 df\n");
    drive_forward(atmega, may_log);
    _delay_ms(100);

    if (!wall_detected) {
      wall_phase = 3;
    }
    return;
  } else if (wall_phase == 3) {
    // turn left phase
    Menu_log(LOG_INFO, "phase 3 tl\n");
    turn_left(atmega, may_log);
    _delay_ms(100);

    if (wall_detected) {
      wall_phase = 2;
    }
    return;
  }

  if (left && mid && right && !seeing_start && may_see_start) {
    seeing_start = true;
    may_see_start = false;
    time_seeing_start = micros();
  } else if (!left || !mid || !right) {
    seeing_start = false;
    may_see_start = true;
  }

  // if (seeing_start) {
  //   Menu_log(LOG_INFO, FMT("start for %d ms\n",
  //                          (int)((micros() - time_seeing_start) / 1000)));
  // }

  if (seeing_start && (micros() - time_seeing_start) / 1000 >= 50) {
    rounds++;

    if (rounds == 4) {
      rounds = 0;
      System_stop(atmega);
      Menu_log(LOG_INFO, "Reset in 5 secs...\n");
      _delay_ms(1000);
      watchdog_init(SEC_4);
      _delay_ms(5000);
      return;
    }

    Menu_log(LOG_INFO, FMT("Round %d/3\n", rounds));
    seeing_start = false;
  }

  // there are rare cases where 011 -> 010 -> 000 is detected so
  // we need to memorize the x0x side and turn the car afterwards
  // so it doesn't drive away from the black line
  State memorized_state = state == STATE_DRV_FW_ML   ? STATE_DRV_FW_ML
                          : state == STATE_DRV_FW_MR ? STATE_DRV_FW_MR
                                                     : STATE_DRV_FW;

  // update state
  if (mid && left) {
    state = STATE_DRV_FW_ML;
  } else if (mid && right) {
    state = STATE_DRV_FW_MR;
  } else if (mid) {
    state = memorized_state;
  } else if (left) {
    state = STATE_TRN_LEFT;
  } else if (right) {
    state = STATE_TRN_RIGHT;
  } else if (state == STATE_DRV_FW_ML) {
    state = STATE_TRN_LEFT;
  } else if (state == STATE_DRV_FW_MR) {
    state = STATE_TRN_RIGHT;
  }

  // execute state
  switch (state) {
    case STATE_LNF:        // fall through
    case STATE_DRV_FW:     // fall through
    case STATE_DRV_FW_ML:  // fall through
    case STATE_DRV_FW_MR:
      drive_forward(atmega, may_log);
      break;
    case STATE_TRN_LEFT:
      turn_left(atmega, may_log);
      break;
    case STATE_TRN_RIGHT:
      turn_right(atmega, may_log);
      break;
  }

  lleft = left;
  lmid = mid;
  lright = right;
}

void turn_left(System* atmega, bool may_log) {
  if (may_log) Menu_log(LOG_DEBUG, TURN_LEFT_MESSAGE);
  Motor_drive_backward(&atmega->mt_left, SPEED_TURN);
  Motor_drive_forward(&atmega->mt_right, SPEED_TURN);
}

void turn_right(System* atmega, bool may_log) {
  if (may_log) Menu_log(LOG_DEBUG, TURN_RIGHT_MESSAGE);
  Motor_drive_forward(&atmega->mt_left, SPEED_TURN);
  Motor_drive_backward(&atmega->mt_right, SPEED_TURN);
}

void drive_forward(System* atmega, bool may_log) {
  if (may_log) Menu_log(LOG_DEBUG, DRIVE_FORWARD_MESSAGE);
  Motor_drive_forward(&atmega->mt_left, SPEED_DRIVE);
  Motor_drive_forward(&atmega->mt_right, SPEED_DRIVE);
}