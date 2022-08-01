#include "Messages.h"

#include "Constants.h"
#include "EventSystem/HardwareTimer.h"
#include "Globals.h"
#include "Models/USART.h"

void show_commands() {
  USART_send_str(USART_instance(), COMMANDS_STR);
  if (presentation_state == PS_DRIVING) {
    USART_send_str(USART_instance(), DRIVING_COMMANDS_STR);
  }
}

void print(const char* message) { USART_send_str(USART_instance(), message); }

void print_rounds_changed_message() {
  switch (rounds) {
    case 0:
    case 1:
      break;
    case 2:
      USART_send_str(USART_instance(), START_ROUND_TWO_MESSAGE);
      break;
    case 3:
      USART_send_str(USART_instance(), START_ROUND_THREE_MESSAGE);
      break;
    case 4:
    default:
      USART_send_str(USART_instance(), END_MESSAGE);
      return;
  }
}

void print_message_with_led_line(System* atmega, const char* message,
                                 uint16_t led_freq) {
  static bool ll_left = true, ll_mid = false, ll_right = false, to_right = true;
  System_stop(atmega);

  if ((millis() - last_led_update) >= led_freq) /*8 Hz*/ {
    last_led_update = millis();
    ShiftRegister_write_n(&atmega->led_strip, 3, ll_left, ll_mid, ll_right);
    if (to_right) {
      ll_right = ll_mid;
      ll_mid = ll_left;
      ll_left = false;
      if (ll_right) {
        to_right = false;
      }
    } else {
      ll_left = ll_mid;
      ll_mid = ll_right;
      ll_right = false;
      if (ll_left) {
        to_right = true;
      }
    }
  }

  if ((millis() - last_message_sent) >= ONE_SECONDS_MS) {
    last_message_sent = millis();
    USART_send_str(USART_instance(), message);
  }
}