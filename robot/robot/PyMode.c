#include "PyMode.h"

#include <stdlib.h>

#include "Driving.h"
#include "Globals.h"
#include "Messages.h"
#include "Reset.h"

void run_py_mode(System* atmega) {
  static bool started = false;

  if (!started) {
    print("OK\n");
    started = true;
  }

  if (current_command != NULL) {
    print(FMT("command: '%s'", current_command));
  }

  if (current_command != NULL && String_contains(current_command, "fw")) {
    drive_forward(atmega);
    print("OK\n");
  } else if (current_command != NULL &&
             String_contains(current_command, "bw")) {
    drive_backward(atmega);
    print("OK\n");
  } else if (current_command != NULL &&
             String_contains(current_command, "lt")) {
    turn_left(atmega);
    print("OK\n");
  } else if (current_command != NULL &&
             String_contains(current_command, "rt")) {
    turn_right(atmega);
    print("OK\n");
  } else if (current_command != NULL &&
             String_contains(current_command, "stop")) {
    System_stop(atmega);
    print("OK\n");
  } else if (current_command != NULL &&
             String_contains(current_command, "reset")) {
    print("OK\n");
    reset_system_now(atmega);
  } else if (current_command != NULL &&
             String_contains(current_command, "sv:")) {
    const char* angle_str = current_command;
    while ((*angle_str) != ':') {
      angle_str++;
    }

    int8_t angle = atoi(angle_str);
    Servo_set_angle(&atmega->us_servo, angle);
    print("OK\n");
  }

  free(current_command);
  current_command = NULL;
}