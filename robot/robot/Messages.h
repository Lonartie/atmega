#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

#include "Models/System.h"

/// @brief shows the list of available commands
void show_commands();

/// @brief prints the given message
/// @param message the message to print
void print(const char* message);

/// @brief shows the message when driving over start block
void print_rounds_changed_message();

/// @brief shows a message every 1 second and lets the leds run up and down with
/// given requency
/// @param atmega the system with the leds
/// @param message the message to show every second
/// @param led_freq the frequency of the leds changing one by one
void print_message_with_led_line(System* atmega, const char* message,
                                 uint16_t led_freq);

#endif  // MESSAGES_H