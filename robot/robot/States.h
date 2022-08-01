#ifndef STATES_H
#define STATES_H

/// @brief the state to memorize the direction to move in
typedef enum DriveDirectionState {
  DDS_LNF,        // line not found
  DDS_DRV_FW,     // driving forward
  DDS_DRV_FW_ML,  // driving forward while middle and left sensor are on
  DDS_DRV_FW_MR,  // driving forward while middle and right sensor are on
  DDS_TRN_LEFT,   // turning left
  DDS_TRN_RIGHT   // turning right
} DriveDirectionState;

/// @brief the recognized track direction (last turn direction)
typedef enum TrackDirection {
  TD_UNKNOWN,  // track direction not known
  TD_LEFT,     // track direction is left
  TD_RIGHT     // track direction is right
} TrackDirection;

/// @brief the state the robot is in while following the task flow of the
/// assignment
typedef enum PresentationState {
  PS_IDLE,            // robot hasn't started yet and doesn't see start block
  PS_ON_START_BLOCK,  // robot is on the start block
  PS_PAUSE,           // robot pauses until unpaused
  PS_DRIVING,         // robot is driving normally
  PS_END              // robot is on the start block and has driven all rounds
} PresentationState;

#endif  // STATES_H