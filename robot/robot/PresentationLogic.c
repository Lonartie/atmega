#include "PresentationLogic.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "Constants.h"
#include "Driving.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/HardwareTimer.h"
#include "Messages.h"
#include "Models/System.h"
#include "ObstacleAvoidance.h"
#include "Reset.h"
#include "States.h"

void idle_state(System* atmega, bool left, bool mid, bool right);
void on_start_block(System* atmega, bool left, bool mid, bool right);
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
    start_driving_time = millis();
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