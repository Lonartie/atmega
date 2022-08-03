#include "Driving.h"

#include "Constants.h"
#include "EventSystem/HardwareTimer.h"
#include "Globals.h"
#include "Messages.h"
#include "ObstacleAvoidance.h"
#include "Reset.h"
#include "States.h"

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

void drive_forward(System* atmega, bool left, bool mid, bool right) {
  smooth_steer_start = millis();
  last_direction_update = millis();
  uint8_t speed = SPEED_DRIVE;
  if (left && mid && right) {
    // drive slower on start block to not overshoot
    speed = SPEED_DRIVE_SLOW;
  }
  Motor_drive_forward(&atmega->mt_left, SPEED_DRIVE);
  Motor_drive_forward(&atmega->mt_right, SPEED_DRIVE);
}

void drive_backward(System* atmega) {
  smooth_steer_start = millis();
  last_direction_update = millis();
  Motor_drive_backward(&atmega->mt_left, SPEED_DRIVE);
  Motor_drive_backward(&atmega->mt_right, SPEED_DRIVE);
}

void stop_driving(System* atmega) {
  last_direction_update = millis();
  Motor_drive_forward(&atmega->mt_left, 0);
  Motor_drive_forward(&atmega->mt_right, 0);
}

void drive(System* atmega, bool left, bool mid, bool right, bool sees_wall) {
  static DriveDirectionState state = DDS_LNF;
  static bool lleft = false, lmid = false, lright = false;

  if (left != lleft || mid != lmid || right != lright) {
    ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);
  }

  if (avoid_obstacles_enabled && (wall_phase != 0 || sees_wall)) {
    avoid_obstacle_logic(atmega, sees_wall, (left || mid || right));
    return;
  }

  count_rounds(atmega, left, mid, right);

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
      drive_forward(atmega, left, mid, right);
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

void count_rounds(System* atmega, bool left, bool mid, bool right) {
  static bool may_see_start = true;
  static bool seeing_start = false;
  static uint32_t time_seeing_start = 0;

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
      print_rounds_changed_message();
      if (rounds >= 4) {
        presentation_state = PS_END;
      }
      seeing_start = false;
    }

    if ((millis() - last_message_sent) >= ONE_SECONDS_MS && rounds > 0) {
      last_message_sent = millis();
      print(FMT(ROUND_MESSAGE, rounds));
    }
  } else {
    if ((millis() - last_message_sent) >= ONE_SECONDS_MS) {
      last_message_sent = millis();
      print(RETURN_HOME_MESSAGE);
    }
    if (seeing_start &&
        (millis() - time_seeing_start) >= TIME_TO_RECOGNIZE_START_BLOCK_MS) {
      print(RETURN_HOME_END_MESSAGE);
      reset_system_now(atmega);
    }
  }
}