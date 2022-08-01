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

  if (current_command != NULL && String_contains(current_command, "forward")) {
    drive_forward(atmega);
  }
}