#include "DrivingLogic.h"

#include <stdbool.h>
#include <util/delay.h>

#include "EventSystem/EventSystem.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/Menu.h"
#include "Models/System.h"
#include "Models/WatchDog.h"

const int8_t SPEED_DRIVE_SLOW = 0;
const int16_t SPEED_DRIVE = 220;
const int16_t SPEED_TURN_A = 220;
const int16_t SPEED_TURN_B = 220;
const int16_t SPEED_TURN_SLOW_A = 240;
const int16_t SPEED_TURN_SLOW_B = 20;

const uint32_t TIME_TO_TRY_SMOOTH_STEERING_US = 150000;

const uint8_t US_SENSOR_DISTANCE_SMALL = 16;
const uint8_t US_SENSOR_DISTANCE_LARGE = 23;
static uint8_t US_CURRENT_SENSOR_DISTANCE = 16;

const uint32_t DIRECTION_UPDATE_DELAY_US = 300000;

const uint16_t MEASURE_THRESHOLD_LEFT = 330;
const uint16_t MEASURE_THRESHOLD_MID = 400;
const uint16_t MEASURE_THRESHOLD_RIGHT = 330;

const char* TURN_LEFT_MESSAGE = "tl\n";
const char* TURN_RIGHT_MESSAGE = "tr\n";
const char* DRIVE_FORWARD_MESSAGE = "df\n";
const char* STOP_MESSAGE = "s\n";
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

typedef enum TrackDirection {
  TRACK_UNKNOWN,
  TRACK_LEFT,
  TRACK_RIGHT
} TrackDirection;

static TrackDirection track_direction = TRACK_UNKNOWN;
static bool wall_detected = false;
static uint32_t last_direction_update = 0;
static bool update_track_direction = true;
static uint8_t wall_phase = 0;
static uint8_t last_wall_phase = UINT8_MAX;
static uint32_t last_measure = 0;
static uint32_t last_last_measure = 0;
static uint32_t smooth_steer_start = 0;
static bool is_smooth_steering = false;

void drive_logic(System* atmega);
void avoid_obstacle_logic(System* atmega, bool sees_wall, bool may_log,
                          bool any_sensor);

void turn_left(System* atmega, bool may_log);
void turn_right(System* atmega, bool may_log);
void turn_smooth_left(System* atmega, bool may_log);
void turn_smooth_right(System* atmega, bool may_log);
void drive_forward(System* atmega, bool may_log);
void stop_driving(System* atmega, bool may_log);

bool measure_was_recently();

void detect_wall(void* system) {
  System* atmega = (System*)system;

  if (UltraSoundSensor_dist(&atmega->us) > US_CURRENT_SENSOR_DISTANCE) {
    wall_detected = false;
    return;
  }

  wall_detected = ((micros() - last_last_measure) < 20000);
  last_last_measure = last_measure;
  last_measure = micros();
}

void Logic_reset(void* system) {
  System* atmega = (System*)system;
  Servo_set_angle(&atmega->us_servo, 0);
  Servo_set_angle(&atmega->us_servo, 0);
  _delay_us(500000);
  US_CURRENT_SENSOR_DISTANCE = US_SENSOR_DISTANCE_SMALL;
  wall_phase = 0;
  last_wall_phase = UINT8_MAX;
  update_track_direction = true;
  last_direction_update = micros();
}

void Logic_start(void* system) {
  System* atmega = (System*)system;
  Servo_set_angle(&atmega->us_servo, 0);

  UltraSoundSensor_set_event(&atmega->us, US_CURRENT_SENSOR_DISTANCE,
                             "US_SENSOR");
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
  // static uint32_t last_time = 0;
  // static uint32_t _time_ = 0;
  static bool may_see_start = true;
  static bool seeing_start = false;
  static uint32_t time_seeing_start = 0;
  static uint8_t rounds = 0;
  // static bool log_1_sec = false;

  // if (micros() - _time_ >= 500000) {
  // if (false) {
  //   // Menu_log(LOG_INFO, FMT("wall: %d\n", wall_detected));
  //   // _time_ = micros();
  //   log_1_sec = true;
  // } else {
  //   log_1_sec = false;
  // }

  if (!atmega->started) {
    Menu_log(LOG_INFO, "not started\n");
    return;
  }

  uint16_t left_measure = ADCPin_read_avg(&atmega->lf_left, 10);
  uint16_t mid_measure = ADCPin_read_avg(&atmega->lf_middle, 10);
  uint16_t right_measure = ADCPin_read_avg(&atmega->lf_right, 10);

  bool left = left_measure > MEASURE_THRESHOLD_LEFT;
  bool mid = mid_measure > MEASURE_THRESHOLD_MID;
  bool right = right_measure > MEASURE_THRESHOLD_RIGHT;
  bool sees_wall = (wall_detected && measure_was_recently());

  bool may_log = false;

  if (left != lleft || mid != lmid || right != lright) {
    // update lights and sends log messages
    may_log = true;
    ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);
    // new_time = micros();
    // uint32_t time_diff = (new_time - last_time) / 1000;
    // last_time = new_time;
    // Menu_log(LOG_DEBUG, FMT(TIMER_MESSAGE, time_diff));
    // Menu_log(LOG_INFO, FMT(SENSORS_MESSAGE, left, mid, right));
    // Menu_log(LOG_DEBUG, FMT(SENSORS_DEBUG_MESSAGE, (int)left_measure,
    //                         (int)mid_measure, (int)right_measure));
  }

  if (wall_phase != 0 || sees_wall) {
    return avoid_obstacle_logic(atmega, sees_wall, may_log,
                                (left || mid || right));
  }

  if (left && mid && right && !seeing_start && may_see_start) {
    seeing_start = true;
    may_see_start = false;
    time_seeing_start = micros();
  } else if (!left || !mid || !right) {
    seeing_start = false;
    may_see_start = true;
  }

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

void obstacle_phase_reset(System* atmega);
void obstacle_phase_0(System* atmega, bool sees_wall, bool may_log);
void obstacle_phase_1(System* atmega, bool sees_wall, bool may_log);
void obstacle_phase_2(System* atmega, bool sees_wall, bool may_log);
void obstacle_phase_3(System* atmega, bool sees_wall, bool may_log);
void obstacle_phase_4(System* atmega, bool sees_wall, bool may_log);

void avoid_obstacle_logic(System* atmega, bool sees_wall, bool may_log,
                          bool any_sensor) {
  if (wall_phase >= 3 && any_sensor) {
    obstacle_phase_reset(atmega);
  } else if (sees_wall && wall_phase == 0) {
    obstacle_phase_0(atmega, sees_wall, may_log);
  } else if (wall_phase == 1) {
    obstacle_phase_1(atmega, sees_wall, may_log);
  } else if (wall_phase == 2) {
    obstacle_phase_2(atmega, sees_wall, may_log);
  } else if (wall_phase == 3) {
    obstacle_phase_3(atmega, sees_wall, may_log);
  } else if (wall_phase == 4) {
    obstacle_phase_4(atmega, sees_wall, may_log);
  }
}

void obstacle_phase_reset(System* atmega) {
  Menu_log(LOG_INFO, "found track again\n");
  Servo_set_angle(&atmega->us_servo, 0);
  // we don't want smooth steering here
  smooth_steer_start = (micros() - 1000000);
  if (track_direction == TRACK_RIGHT) {
    turn_right(atmega, true);
  } else {
    turn_left(atmega, true);
  }
  _delay_us(100000);
  wall_phase = 0;
  last_wall_phase = UINT8_MAX;
  US_CURRENT_SENSOR_DISTANCE = US_SENSOR_DISTANCE_SMALL;
  update_track_direction = true;
  last_direction_update = micros();
}

void obstacle_phase_0(System* atmega, bool sees_wall, bool may_log) {
  // setup phase
  if (last_wall_phase != wall_phase) {
    update_track_direction = false;
    if (track_direction == TRACK_RIGHT) {
      Servo_set_angle(&atmega->us_servo, -90);
    } else {
      Servo_set_angle(&atmega->us_servo, 90);
    }
    stop_driving(atmega, may_log);
    _delay_us(100000);
    last_wall_phase = wall_phase;
    wall_phase = 1;
  }

  if (!sees_wall) {
    US_CURRENT_SENSOR_DISTANCE = US_SENSOR_DISTANCE_LARGE;
    wall_phase = 2;
  }
}

void obstacle_phase_1(System* atmega, bool sees_wall, bool may_log) {
  obstacle_phase_0(atmega, sees_wall, may_log);
}

void obstacle_phase_2(System* atmega, bool sees_wall, bool may_log) {
  // turn right phase
  if (last_wall_phase != wall_phase) {
    if (track_direction == TRACK_RIGHT) {
      turn_right(atmega, may_log);
    } else {
      turn_left(atmega, may_log);
    }
    last_wall_phase = wall_phase;
  }

  if (sees_wall) {
    _delay_us(100000);
    wall_phase = 3;
  }
}

void obstacle_phase_3(System* atmega, bool sees_wall, bool may_log) {
  // drive forward phase
  if (last_wall_phase != wall_phase &&
      UltraSoundSensor_dist(&atmega->us) == US_CURRENT_SENSOR_DISTANCE) {
    drive_forward(atmega, may_log);
    last_wall_phase = wall_phase;
  } else if (UltraSoundSensor_dist(&atmega->us) < US_CURRENT_SENSOR_DISTANCE) {
    if (track_direction == TRACK_RIGHT) {
      turn_smooth_right(atmega, may_log);
    } else {
      turn_smooth_left(atmega, may_log);
    }
    last_wall_phase = wall_phase;
  } else if (UltraSoundSensor_dist(&atmega->us) > US_CURRENT_SENSOR_DISTANCE) {
    if (track_direction == TRACK_RIGHT) {
      turn_smooth_left(atmega, may_log);
    } else {
      turn_smooth_right(atmega, may_log);
    }
    last_wall_phase = wall_phase;
  }

  if (!sees_wall) {
    _delay_us(100000);
    wall_phase = 4;
  }
}

void obstacle_phase_4(System* atmega, bool sees_wall, bool may_log) {
  // turn left phase
  if (last_wall_phase != wall_phase) {
    Menu_log(LOG_INFO, "p4\n");
    if (track_direction == TRACK_RIGHT) {
      turn_smooth_left(atmega, may_log);
    } else {
      turn_smooth_right(atmega, may_log);
    }
    last_wall_phase = wall_phase;
  }

  if (sees_wall) {
    _delay_us(100000);
    wall_phase = 3;
  }
}

void turn_left(System* atmega, bool may_log MAYBE_UNUSED) {
  if ((micros() - last_direction_update) >= DIRECTION_UPDATE_DELAY_US &&
      update_track_direction) {
    Menu_log(LOG_DEBUG, "now!\n");
    last_direction_update = micros();
    track_direction = TRACK_LEFT;
  }
  // try smooth steer first
  if ((micros() - smooth_steer_start) < TIME_TO_TRY_SMOOTH_STEERING_US) {
    if (!is_smooth_steering) {
      is_smooth_steering = true;
      smooth_steer_start = micros();
    }
    turn_smooth_left(atmega, may_log);
  } else {
    is_smooth_steering = false;
    Motor_drive_backward(&atmega->mt_left, SPEED_TURN_B);
    Motor_drive_forward(&atmega->mt_right, SPEED_TURN_A);
  }
}

void turn_right(System* atmega, bool may_log MAYBE_UNUSED) {
  if ((micros() - last_direction_update) >= DIRECTION_UPDATE_DELAY_US &&
      update_track_direction) {
    Menu_log(LOG_DEBUG, "now!\n");
    last_direction_update = micros();
    track_direction = TRACK_RIGHT;
  }

  // try smooth steer first
  if ((micros() - smooth_steer_start) < TIME_TO_TRY_SMOOTH_STEERING_US) {
    if (!is_smooth_steering) {
      is_smooth_steering = true;
      smooth_steer_start = micros();
    }
    turn_smooth_right(atmega, may_log);
  } else {
    is_smooth_steering = false;
    Motor_drive_forward(&atmega->mt_left, SPEED_TURN_A);
    Motor_drive_backward(&atmega->mt_right, SPEED_TURN_B);
  }
}

void turn_smooth_left(System* atmega, bool may_log) {
  if (may_log) Menu_log(LOG_DEBUG, TURN_LEFT_MESSAGE);
  last_direction_update = micros();
  Motor_drive_forward(&atmega->mt_right, SPEED_TURN_SLOW_A);
  Motor_drive_backward(&atmega->mt_left, SPEED_TURN_SLOW_B);
}

void turn_smooth_right(System* atmega, bool may_log) {
  if (may_log) Menu_log(LOG_DEBUG, TURN_RIGHT_MESSAGE);
  last_direction_update = micros();
  Motor_drive_forward(&atmega->mt_left, SPEED_TURN_SLOW_A);
  Motor_drive_backward(&atmega->mt_right, SPEED_TURN_SLOW_B);
}

void drive_forward(System* atmega, bool may_log) {
  smooth_steer_start = micros();
  if (may_log) Menu_log(LOG_DEBUG, DRIVE_FORWARD_MESSAGE);
  last_direction_update = micros();
  Motor_drive_forward(&atmega->mt_left, SPEED_DRIVE);
  Motor_drive_forward(&atmega->mt_right, SPEED_DRIVE);
}

void stop_driving(System* atmega, bool may_log) {
  if (may_log) Menu_log(LOG_DEBUG, STOP_MESSAGE);
  last_direction_update = micros();
  Motor_drive_forward(&atmega->mt_left, 0);
  Motor_drive_forward(&atmega->mt_right, 0);
}

bool measure_was_recently() { return (micros() - last_measure) <= 50000; }