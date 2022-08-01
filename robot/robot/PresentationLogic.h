#ifndef DRIVING_LOGIC_H
#define DRIVING_LOGIC_H

#include "Models/System.h"

/// @brief handles the input commands and calls the corresponding functions
/// @param usart the usart (sender)
void presentation_handle_command(void* usart);

// /// @brief resets the
// void presentation_reset(void* system);

/// @brief starts the presentation mode by connecting events and initializing
/// servo to default state
/// @param system the system to drive
void presentation_start(void* system);

/// @brief updates the system by calling the driving logic
/// @note this method keeps track of the different phases of the presentation
/// flow
/// @param system the system to drive
void presentation_update(void* system);

/// @brief processes the idle state when not on starting block
/// @param atmega the system to drive
/// @param left whether or not the left line sensor sees something
/// @param mid whether or not the mid line sensor sees something
/// @param right whether or not the right line sensor sees something
void presentation_process_idle_state(System* atmega, bool left, bool mid,
                                     bool right);

/// @brief processes the on-start-block state
/// @param atmega the system to drive
/// @param left whether or not the left line sensor sees something
/// @param mid whether or not the mid line sensor sees something
/// @param right whether or not the right line sensor sees something
void presentation_process_on_start_block(System* atmega, bool left, bool mid,
                                         bool right);

/// @brief checks whether or not to toggle pause state
/// @param atmega the system to drive
void presentation_check_toggle_drive_pause(System* atmega);

/// @brief checks whether or not to return home
void presentation_check_return_home();

#endif  // DRIVING_LOGIC_H