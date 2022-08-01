#include "PresentationLogic.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "Constants.h"
#include "Driving.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/HardwareTimer.h"
#include "Globals.h"
#include "Messages.h"
#include "Models/System.h"
#include "ObstacleAvoidance.h"
#include "Reset.h"
#include "States.h"

void presentation_handle_command(void* usart) {
  if (current_command != NULL) {
    free(current_command);
  }
  current_command = strdup(((USART*)usart)->data);
}

void presentation_start(void* system) {
  System* atmega = (System*)system;
  Servo_set_angle(&atmega->us_servo, 0);
  UltraSoundSensor_set_event(&atmega->us, us_current_sensor_distance,
                             "US_SENSOR");
  EventSystem_reg_listener(EventSystem_instance(),
                           Listener_create_r(system, detect_wall, "US_SENSOR"));
}

void presentation_update(void* system) {
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
    print_message_with_led_line(atmega, SAFE_SATE_MESSAGE, 125);
    return;
  }

  if (current_command != NULL && String_equals(current_command, "?")) {
    show_commands();
    free(current_command);
    current_command = NULL;
    return;
  } else if (current_command != NULL && String_equals(current_command, "R")) {
    print(MANUAL_RESET_MESSAGE);
    free(current_command);
    current_command = NULL;
    reset_system_5_seconds(atmega);  // no-return
  } else if (current_command != NULL && String_equals(current_command, "X")) {
    safe_state = true;
    free(current_command);
    current_command = NULL;
    return;
  } else if (current_command != NULL && String_equals(current_command, "A")) {
    avoid_obstacles_enabled = !avoid_obstacles_enabled;
    print(FMT("avoiding obstacles %s\n",
              (avoid_obstacles_enabled ? "enabled" : "disabled")));
    free(current_command);
    current_command = NULL;
  }

  switch (presentation_state) {
    case PS_IDLE:
      presentation_process_idle_state(atmega, left, mid, right);
      break;
    case PS_ON_START_BLOCK:
      presentation_process_on_start_block(atmega, left, mid, right);
      break;
    case PS_PAUSE:
      presentation_check_toggle_drive_pause(atmega);
      print_message_with_led_line(atmega, PAUSE_MESSAGE, 500);
      break;
    case PS_DRIVING:
      presentation_check_toggle_drive_pause(atmega);
      presentation_check_return_home();
      drive(atmega, left, mid, right, sees_wall);
      break;
    case PS_END:
      rounds = 0;
      reset_system_5_seconds(atmega);  // no-return
      break;
  }
}

void presentation_process_idle_state(System* atmega, bool left, bool mid,
                                     bool right) {
  ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);

  if (left && mid && right) {
    start_driving_time = millis();
    presentation_state = PS_ON_START_BLOCK;
    return;
  }

  if ((millis() - last_message_sent) >= ONE_SECONDS_MS) {
    last_message_sent = millis();
    print(IDLE_MESSAGE);
  }
}

void presentation_process_on_start_block(System* atmega, bool left, bool mid,
                                         bool right) {
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

  if (current_command != NULL && String_equals(current_command, "S")) {
    presentation_state = PS_DRIVING;
    free(current_command);
    current_command = NULL;
    USART_send_str(USART_instance(), START_ROUND_ONE_MESSAGE);
    return;
  }

  if ((millis() - last_message_sent) >= ONE_SECONDS_MS) {
    last_message_sent = millis();
    USART_send_str(USART_instance(), START_BLOCK_MESSAGE);
  }
}

void presentation_check_toggle_drive_pause(System* atmega) {
  if (current_command != NULL && String_equals(current_command, "P")) {
    free(current_command);
    current_command = NULL;
    presentation_state =
        (presentation_state == PS_DRIVING ? PS_PAUSE : PS_DRIVING);
    System_start(atmega);
    return;
  }
}

void presentation_check_return_home() {
  if (current_command != NULL && String_equals(current_command, "C")) {
    free(current_command);
    current_command = NULL;
    return_home = true;
    return;
  }
}