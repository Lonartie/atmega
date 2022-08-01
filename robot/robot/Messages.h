#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

#include "Models/System.h"

// shows the list of available commands
void show_commands();

void print_rounds_changed_message();

void print_rounds_message();

void print_return_home_message();

void print_arrived_home_message();

void print_message_with_led_line(System* atmega, const char* message,
                                 uint16_t led_freq);

#endif  // MESSAGES_H