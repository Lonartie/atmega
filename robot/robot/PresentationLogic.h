#ifndef DRIVING_LOGIC_H
#define DRIVING_LOGIC_H

#include "Models/System.h"

void presentation_handle_command(void* usart);

void presentation_reset(void* system);
void presentation_start(void* system);

void presentation_update(void* system);

void presentation_process_idle_state(System* atmega, bool left, bool mid,
                                     bool right);
void presentation_process_on_start_block(System* atmega, bool left, bool mid,
                                         bool right);
void presentation_check_toggle_drive_pause(System* atmega);
void presentation_check_return_home();

#endif  // DRIVING_LOGIC_H