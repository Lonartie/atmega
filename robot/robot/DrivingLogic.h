#ifndef DRIVING_LOGIC_H
#define DRIVING_LOGIC_H

void Logic_command(void* usart);

void Logic_reset(void* system);
void Logic_start(void* system);

void Logic_drive_3_rounds(void* system);

#endif  // DRIVING_LOGIC_H