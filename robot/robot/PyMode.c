#include "PyMode.h"

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

  if (current_command != NULL && String_contains(current_command, "stop")) {
    System_stop(atmega);
    print("OK\n");
  }
}