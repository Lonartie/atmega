#include "DrivingLogic.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "Constants.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/System.h"
#include "Models/USART.h"
#include "Models/WatchDog.h"
#include "States.h"

void avoid_obstacle_logic(System* atmega, bool sees_wall, bool any_sensor);
void turn_left(System* atmega);
void turn_right(System* atmega);
void turn_smooth_left(System* atmega);
void turn_smooth_right(System* atmega);
void drive_forward(System* atmega);
void stop_driving(System* atmega);
bool measure_was_recently();
void idle_state(System* atmega, bool left, bool mid, bool right);
void on_start_block(System* atmega, bool left, bool mid, bool right);
void reset_system_5_seconds(System* atmega);
void reset_system_now(System* atmega);
void drive(System* atmega, bool left, bool mid, bool right, bool sees_wall);
void show_commands();
void message_led_line(System* atmega, const char* message, uint16_t led_freq);
void check_toggle_drive_pause(System* atmega);
void check_return_home();

void Logic_command(void* usart) {
  if (current_command != NULL) {
    free(current_command);
  }
  current_command = strdup(((USART*)usart)->data);
}

void detect_wall(void* system) {
  System* atmega = (System*)system;

  if (UltraSoundSensor_dist(&atmega->us) > US_CURRENT_SENSOR_DISTANCE) {
    wall_detected = false;
    return;
  }

  wall_detected = ((millis() - last_last_measure) < 20);
  last_last_measure = last_measure;
  last_measure = millis();
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
  last_direction_update = millis();
}

void Logic_start(void* system) {
  System* atmega = (System*)system;
  Servo_set_angle(&atmega->us_servo, 0);

  UltraSoundSensor_set_event(&atmega->us, US_CURRENT_SENSOR_DISTANCE,
                             "US_SENSOR");
  EventSystem_reg_listener(EventSystem_instance(),
                           Listener_create_r(system, detect_wall, "US_SENSOR"));
}

void Logic_drive_3_rounds(void* system) {
  System* atmega = (System*)system;

  if (!atmega->started) {
    System_start(atmega);
    return;
  }

  uint16_t left_measure = ADCPin_read_avg(&atmega->lf_left, 10);
  uint16_t mid_measure = ADCPin_read_avg(&atmega->lf_middle, 10);
  uint16_t right_measure = ADCPin_read_avg(&atmega->lf_right, 10);

  bool left = left_measure > MEASURE_THRESHOLD_LEFT;
  bool mid = mid_measure > MEASURE_THRESHOLD_MID;
  bool right = right_measure > MEASURE_THRESHOLD_RIGHT;
  bool sees_wall = (wall_detected && measure_was_recently());

  if (safe_state) {
    message_led_line(atmega, SAFE_SATE_MESSAGE, 125);
    return;
  }

  if (current_command != NULL && strcmp(current_command, "?") == 0) {
    show_commands();
    free(current_command);
    current_command = NULL;
    return;
  } else if (current_command != NULL && strcmp(current_command, "R") == 0) {
    USART_send_str(USART_instance(), MANUAL_RESET_MESSAGE);
    free(current_command);
    current_command = NULL;
    reset_system_5_seconds(atmega);  // no-return
  } else if (current_command != NULL && strcmp(current_command, "X") == 0) {
    safe_state = true;
    free(current_command);
    current_command = NULL;
    return;
  } else if (current_command != NULL && strcmp(current_command, "A") == 0) {
    avoid_obstacles_enabled = !avoid_obstacles_enabled;
    USART_send_str(USART_instance(),
                   FMT("avoiding obstacles %s\n",
                       (avoid_obstacles_enabled ? "enabled" : "disabled")));
    free(current_command);
    current_command = NULL;
  }

  switch (presentation_state) {
    case PS_IDLE:
      idle_state(atmega, left, mid, right);
      break;
    case PS_ON_START_BLOCK:
      on_start_block(atmega, left, mid, right);
      break;
    case PS_PAUSE:
      check_toggle_drive_pause(atmega);
      message_led_line(atmega, PAUSE_MESSAGE, 2000);
      break;
    case DRIVING:
      check_toggle_drive_pause(atmega);
      check_return_home();
      drive(atmega, left, mid, right, sees_wall);
      break;
    case PS_END:
      rounds = 0;
      reset_system_5_seconds(atmega);  // no-return
      break;
  }
}

void idle_state(System* atmega, bool left, bool mid, bool right) {
  ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);

  if (left && mid && right) {
    presentation_state = PS_ON_START_BLOCK;
    return;
  }

  if ((millis() - last_message_sent) >= one_seconds_ms) {
    last_message_sent = millis();
    USART_send_str(USART_instance(), IDLE_MESSAGE);
  }
}

void on_start_block(System* atmega, bool left, bool mid, bool right) {
  static bool led_on = false;

  if ((millis() - last_led_blink) >= 100) {
    led_on = !led_on;
    last_led_blink = millis();
    ShiftRegister_write_n(&atmega->led_strip, 3, led_on, led_on, led_on);
  }

  if (!left || !mid || !right) {
    presentation_state = PS_IDLE;
    return;
  }

  if (current_command != NULL && strcmp(current_command, "S") == 0) {
    presentation_state = DRIVING;
    free(current_command);
    current_command = NULL;
    USART_send_str(USART_instance(), START_ROUND_ONE_MESSAGE);
    return;
  }

  if ((millis() - last_message_sent) >= one_seconds_ms) {
    last_message_sent = millis();
    USART_send_str(USART_instance(), START_BLOCK_MESSAGE);
  }
}

void drive(System* atmega, bool left, bool mid, bool right, bool sees_wall) {
  static DriveDirectionState state = DDS_LNF;
  static bool lleft = false, lmid = false, lright = false;
  static bool may_see_start = true;
  static bool seeing_start = false;
  static uint32_t time_seeing_start = 0;

  if (left != lleft || mid != lmid || right != lright) {
    ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);
  }

  if (avoid_obstacles_enabled && (wall_phase != 0 || sees_wall)) {
    avoid_obstacle_logic(atmega, sees_wall, (left || mid || right));
    return;
  }

  if (left && mid && right && !seeing_start && may_see_start) {
    seeing_start = true;
    may_see_start = false;
    time_seeing_start = millis();
  } else if (!left || !mid || !right) {
    seeing_start = false;
    may_see_start = true;
  }

  if (!return_home) {
    if (seeing_start &&
        (millis() - time_seeing_start) >= TIME_TO_RECOGNIZE_START_BLOCK_MS) {
      rounds++;
      switch (rounds) {
        case 2:
          USART_send_str(USART_instance(), START_ROUND_TWO_MESSAGE);
          break;
        case 3:
          USART_send_str(USART_instance(), START_ROUND_THREE_MESSAGE);
          break;
        case 4:
          USART_send_str(USART_instance(), END_MESSAGE);
          presentation_state = PS_END;
          return;
      }

      seeing_start = false;
    }

    if ((millis() - last_message_sent) >= one_seconds_ms) {
      last_message_sent = millis();
      USART_send_str(USART_instance(), FMT(ROUND_MESSAGE, rounds));
    }
  } else {
    if ((millis() - last_message_sent) >= one_seconds_ms) {
      last_message_sent = millis();
      USART_send_str(USART_instance(), RETURN_HOME_MESSAGE);
    }
    if (seeing_start &&
        (millis() - time_seeing_start) >= TIME_TO_RECOGNIZE_START_BLOCK_MS) {
      USART_send_str(USART_instance(), RETURN_HOME_END_MESSAGE);
      reset_system_now(atmega);
    }
  }

  // there are rare cases where 011 -> 010 -> 000 is detected so
  // we need to memorize the x0x side and turn the car afterwards
  // so it doesn't drive away from the black line
  DriveDirectionState memorized_state = state == DDS_DRV_FW_ML   ? DDS_DRV_FW_ML
                                        : state == DDS_DRV_FW_MR ? DDS_DRV_FW_MR
                                                                 : DDS_DRV_FW;

  // update state
  if (mid && left) {
    state = DDS_DRV_FW_ML;
  } else if (mid && right) {
    state = DDS_DRV_FW_MR;
  } else if (mid) {
    state = memorized_state;
  } else if (left) {
    state = DDS_TRN_LEFT;
  } else if (right) {
    state = DDS_TRN_RIGHT;
  } else if (state == DDS_DRV_FW_ML) {
    state = DDS_TRN_LEFT;
  } else if (state == DDS_DRV_FW_MR) {
    state = DDS_TRN_RIGHT;
  }

  // execute state
  switch (state) {
    case DDS_LNF:        // fall through
    case DDS_DRV_FW:     // fall through
    case DDS_DRV_FW_ML:  // fall through
    case DDS_DRV_FW_MR:
      drive_forward(atmega);
      break;
    case DDS_TRN_LEFT:
      turn_left(atmega);
      break;
    case DDS_TRN_RIGHT:
      turn_right(atmega);
      break;
  }

  lleft = left;
  lmid = mid;
  lright = right;
}

void obstacle_phase_reset(System* atmega);
void obstacle_phase_0(System* atmega, bool sees_wall);
void obstacle_phase_1(System* atmega, bool sees_wall);
void obstacle_phase_2(System* atmega, bool sees_wall);
void obstacle_phase_3(System* atmega, bool sees_wall);
void obstacle_phase_4(System* atmega, bool sees_wall);

void avoid_obstacle_logic(System* atmega, bool sees_wall, bool any_sensor) {
  if (wall_phase >= 3 && any_sensor) {
    obstacle_phase_reset(atmega);
  } else if (sees_wall && wall_phase == 0) {
    obstacle_phase_0(atmega, sees_wall);
  } else if (wall_phase == 1) {
    obstacle_phase_1(atmega, sees_wall);
  } else if (wall_phase == 2) {
    obstacle_phase_2(atmega, sees_wall);
  } else if (wall_phase == 3) {
    obstacle_phase_3(atmega, sees_wall);
  } else if (wall_phase == 4) {
    obstacle_phase_4(atmega, sees_wall);
  }
}

void obstacle_phase_reset(System* atmega) {
  Servo_set_angle(&atmega->us_servo, 0);
  // we don't want smooth steering here
  smooth_steer_start = (millis() - one_seconds_ms);
  if (track_direction == TD_RIGHT) {
    turn_right(atmega);
  } else {
    turn_left(atmega);
  }
  _delay_us(100000);
  wall_phase = 0;
  last_wall_phase = UINT8_MAX;
  US_CURRENT_SENSOR_DISTANCE = US_SENSOR_DISTANCE_SMALL;
  update_track_direction = true;
  last_direction_update = millis();
}

void obstacle_phase_0(System* atmega, bool sees_wall) {
  // setup phase
  if (last_wall_phase != wall_phase) {
    update_track_direction = false;
    if (track_direction == TD_RIGHT) {
      Servo_set_angle(&atmega->us_servo, -90);
    } else {
      Servo_set_angle(&atmega->us_servo, 90);
    }
    stop_driving(atmega);
    _delay_us(100000);
    last_wall_phase = wall_phase;
    wall_phase = 1;
  }

  if (!sees_wall) {
    US_CURRENT_SENSOR_DISTANCE = US_SENSOR_DISTANCE_LARGE;
    wall_phase = 2;
  }
}

void obstacle_phase_1(System* atmega, bool sees_wall) {
  obstacle_phase_0(atmega, sees_wall);
}

void obstacle_phase_2(System* atmega, bool sees_wall) {
  // turn right phase
  if (last_wall_phase != wall_phase) {
    if (track_direction == TD_RIGHT) {
      turn_right(atmega);
    } else {
      turn_left(atmega);
    }
    last_wall_phase = wall_phase;
  }

  if (sees_wall) {
    _delay_us(100000);
    wall_phase = 3;
  }
}

void obstacle_phase_3(System* atmega, bool sees_wall) {
  // drive forward phase
  if (last_wall_phase != wall_phase &&
      UltraSoundSensor_dist(&atmega->us) == US_CURRENT_SENSOR_DISTANCE) {
    drive_forward(atmega);
    last_wall_phase = wall_phase;
  } else if (UltraSoundSensor_dist(&atmega->us) < US_CURRENT_SENSOR_DISTANCE) {
    if (track_direction == TD_RIGHT) {
      turn_smooth_right(atmega);
    } else {
      turn_smooth_left(atmega);
    }
    last_wall_phase = wall_phase;
  } else if (UltraSoundSensor_dist(&atmega->us) > US_CURRENT_SENSOR_DISTANCE) {
    if (track_direction == TD_RIGHT) {
      turn_smooth_left(atmega);
    } else {
      turn_smooth_right(atmega);
    }
    last_wall_phase = wall_phase;
  }

  if (!sees_wall) {
    _delay_us(100000);
    wall_phase = 4;
  }
}

void obstacle_phase_4(System* atmega, bool sees_wall) {
  // turn left phase
  if (last_wall_phase != wall_phase) {
    if (track_direction == TD_RIGHT) {
      turn_smooth_left(atmega);
    } else {
      turn_smooth_right(atmega);
    }
    last_wall_phase = wall_phase;
  }

  if (sees_wall) {
    _delay_us(100000);
    wall_phase = 3;
  }
}

void turn_left(System* atmega) {
  if ((millis() - last_direction_update) >= DIRECTION_UPDATE_DELAY_MS &&
      update_track_direction) {
    last_direction_update = millis();
    track_direction = TD_LEFT;
  }
  // try smooth steer first
  if ((millis() - smooth_steer_start) < TIME_TO_TRY_SMOOTH_STEERING_MS) {
    if (!is_smooth_steering) {
      is_smooth_steering = true;
      smooth_steer_start = millis();
    }
    turn_smooth_left(atmega);
  } else {
    is_smooth_steering = false;
    Motor_drive_backward(&atmega->mt_left, SPEED_TURN_B);
    Motor_drive_forward(&atmega->mt_right, SPEED_TURN_A);
  }
}

void turn_right(System* atmega) {
  if ((millis() - last_direction_update) >= DIRECTION_UPDATE_DELAY_MS &&
      update_track_direction) {
    last_direction_update = millis();
    track_direction = TD_RIGHT;
  }

  // try smooth steer first
  if ((millis() - smooth_steer_start) < TIME_TO_TRY_SMOOTH_STEERING_MS) {
    if (!is_smooth_steering) {
      is_smooth_steering = true;
      smooth_steer_start = millis();
    }
    turn_smooth_right(atmega);
  } else {
    is_smooth_steering = false;
    Motor_drive_forward(&atmega->mt_left, SPEED_TURN_A);
    Motor_drive_backward(&atmega->mt_right, SPEED_TURN_B);
  }
}

void turn_smooth_left(System* atmega) {
  last_direction_update = millis();
  Motor_drive_forward(&atmega->mt_right, SPEED_TURN_SLOW_A);
  Motor_drive_backward(&atmega->mt_left, SPEED_TURN_SLOW_B);
}

void turn_smooth_right(System* atmega) {
  last_direction_update = millis();
  Motor_drive_forward(&atmega->mt_left, SPEED_TURN_SLOW_A);
  Motor_drive_backward(&atmega->mt_right, SPEED_TURN_SLOW_B);
}

void drive_forward(System* atmega) {
  smooth_steer_start = millis();
  last_direction_update = millis();
  Motor_drive_forward(&atmega->mt_left, SPEED_DRIVE);
  Motor_drive_forward(&atmega->mt_right, SPEED_DRIVE);
}

void stop_driving(System* atmega) {
  last_direction_update = millis();
  Motor_drive_forward(&atmega->mt_left, 0);
  Motor_drive_forward(&atmega->mt_right, 0);
}

bool measure_was_recently() { return (millis() - last_measure) <= 50; }

void reset_system_5_seconds(System* atmega) {
  System_stop(atmega);
  _delay_ms(one_seconds_ms);
  watchdog_init(SEC_4);
  _delay_ms(five_seconds_ms);
  return;
}

void reset_system_now(System* atmega) {
  System_stop(atmega);
  watchdog_init(NOW);
  _delay_ms(one_seconds_ms);
  return;
}

void show_commands() {
  USART_send_str(USART_instance(), COMMANDS_STR);
  if (presentation_state == DRIVING) {
    USART_send_str(USART_instance(), DRIVING_COMMANDS_STR);
  }
}

void message_led_line(System* atmega, const char* message, uint16_t led_freq) {
  static bool ll_left = true, ll_mid = false, ll_right = false, to_right = true;
  System_stop(atmega);

  if ((millis() - last_led_update) >= led_freq) /*8 Hz*/ {
    last_led_update = millis();
    ShiftRegister_write_n(&atmega->led_strip, 3, ll_left, ll_mid, ll_right);
    if (to_right) {
      ll_right = ll_mid;
      ll_mid = ll_left;
      ll_left = false;
      if (ll_right) {
        to_right = false;
      }
    } else {
      ll_left = ll_mid;
      ll_mid = ll_right;
      ll_right = false;
      if (ll_left) {
        to_right = true;
      }
    }
  }

  if ((millis() - last_message_sent) >= one_seconds_ms) {
    last_message_sent = millis();
    USART_send_str(USART_instance(), message);
  }
}

void check_toggle_drive_pause(System* atmega) {
  if (current_command != NULL && strcmp(current_command, "P") == 0) {
    free(current_command);
    current_command = NULL;
    presentation_state = (presentation_state == DRIVING ? PS_PAUSE : DRIVING);
    System_start(atmega);
    return;
  }
}

void check_return_home() {
  if (current_command != NULL && strcmp(current_command, "C") == 0) {
    free(current_command);
    current_command = NULL;
    return_home = true;
    return;
  }
}