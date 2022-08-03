#include "PyMode.h"

#include <stdlib.h>

#include "Driving.h"
#include "Globals.h"
#include "Messages.h"

void run_py_mode(System* atmega) {
  static bool started = false;

  if (!started) {
    print("OK\n");
    started = true;
  }

  if (current_command != NULL && String_contains(current_command, "fw")) {
    drive_forward(atmega);
    print("OK\n");
  }

  if (current_command != NULL && String_contains(current_command, "bw")) {
    drive_backward(atmega);
    print("OK\n");
  }

  if (current_command != NULL && String_contains(current_command, "lt")) {
    turn_left(atmega);
    print("OK\n");
  }

  if (current_command != NULL && String_contains(current_command, "rt")) {
    turn_right(atmega);
    print("OK\n");
  }

  if (current_command != NULL && String_contains(current_command, "stop")) {
    System_stop(atmega);
    print("OK\n");
  }

  free(current_command);
  current_command = NULL;
}