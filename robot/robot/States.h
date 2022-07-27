#ifndef STATES_H
#define STATES_H

typedef enum State {
  STATE_LNF,
  STATE_DRV_FW,
  STATE_DRV_FW_ML,
  STATE_DRV_FW_MR,
  STATE_TRN_LEFT,
  STATE_TRN_RIGHT
} State;

typedef enum TrackDirection {
  TRACK_UNKNOWN,
  TRACK_LEFT,
  TRACK_RIGHT
} TrackDirection;

typedef enum PresentationState {
  IDLE,
  ON_START_BLOCK,
  DRIVING_FIRST_ROUND,
  DRIVING_SECOND_ROUND,
  DRIVING_THIRD_ROUND,
  END
} PresentationState;

#endif // STATES_H