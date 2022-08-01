#include "ObstacleAvoidance.h"

#include <util/delay.h>

#include "Constants.h"
#include "Driving.h"
#include "EventSystem/HardwareTimer.h"
#include "Globals.h"

void detect_wall(void* system) {
  System* atmega = (System*)system;

  if (UltraSoundSensor_dist(&atmega->us) > us_current_sensor_distance) {
    wall_detected = false;
    return;
  }

  wall_detected = ((millis() - last_last_measure) < 20);
  last_last_measure = last_measure;
  last_measure = millis();
}

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
  smooth_steer_start = (millis() - ONE_SECONDS_MS);
  if (track_direction == TD_RIGHT) {
    turn_right(atmega);
  } else {
    turn_left(atmega);
  }
  _delay_us(100000);
  wall_phase = 0;
  last_wall_phase = UINT8_MAX;
  us_current_sensor_distance = US_SENSOR_DISTANCE_SMALL;
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
    us_current_sensor_distance = US_SENSOR_DISTANCE_LARGE;
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
      UltraSoundSensor_dist(&atmega->us) == us_current_sensor_distance) {
    drive_forward(atmega);
    last_wall_phase = wall_phase;
  } else if (UltraSoundSensor_dist(&atmega->us) < us_current_sensor_distance) {
    if (track_direction == TD_RIGHT) {
      turn_smooth_right(atmega);
    } else {
      turn_smooth_left(atmega);
    }
    last_wall_phase = wall_phase;
  } else if (UltraSoundSensor_dist(&atmega->us) > us_current_sensor_distance) {
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

bool measure_was_recently() { return (millis() - last_measure) <= 50; }