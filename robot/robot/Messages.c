#include "Messages.h"

#include "Constants.h"
#include "Models/USART.h"

void show_commands() {
  USART_send_str(USART_instance(), COMMANDS_STR);
  if (presentation_state == PS_DRIVING) {
    USART_send_str(USART_instance(), DRIVING_COMMANDS_STR);
  }
}