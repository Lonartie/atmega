#include "iesusart.h"
#include <stdio.h>
#include <stdlib.h>
//#define DEBUG // Warum nicht per Kommandozeile definieren?
#define MSG_WELCOME "\n\nSach ma was!\n"
#define MSG_ECHONUMBER "Uebertragen wurden %d Zeichen, und zwar:\n"
// Wer das hier aufraeumt, gut kommentiert und englisch lokalisiert, bekommt einen Kaffee.

typedef struct rcved {
	unsigned char symbol;
	unsigned char idx;
	struct rcved *next;
} rcv;

char* echo_string_from_list(rcv *start, rcv *end) {
	char *echo_string = malloc(sizeof(char) * end->idx + 2);
	rcv *current_rcv = start;
	while(current_rcv != NULL) {
		echo_string[current_rcv->idx] = current_rcv->symbol;
		current_rcv = current_rcv->next;
	}
	echo_string[end->idx + 1] = '\n';
	echo_string[end->idx + 2] = '\0';
	return echo_string;
}

void print_received(rcv *start, rcv *end) {
	rcv *current = start;
	char string_output_number_of_symbols[100];
	sprintf(string_output_number_of_symbols,
	        MSG_ECHONUMBER,
	        end->idx);
	USART_print(string_output_number_of_symbols);
	char *echo_string = echo_string_from_list(start, end);
	USART_print(echo_string);
	free(echo_string);
}

void serial_echo(void) {
	while(1) {
		unsigned char counter = 0;
		USART_print(MSG_WELCOME);
		rcv *input = malloc(sizeof(rcv));
		rcv *start = input;
		rcv *end;
		while(1) {
			input->symbol = USART_receiveByte();

			#ifdef DEBUG
				char test[3] = {input->symbol, '\n', '\0'};
				USART_print(test);
			#endif

			if (input->symbol == '\r') {
				input->next = NULL;
				input->idx = counter;
				end = input;
				print_received(start, end);
				// Und nun? Wer zuerst das Speicherleck beseitigt,
				// bekommt einen Kaffee!
				break;
			} else {
				input->idx = counter++;
				rcv *next_input = malloc(sizeof(rcv));
				input->next = next_input;
				input = next_input;
			}
		}
	}
}

int main(void) {
	USART_init(UBRR_SETTING);
	serial_echo();
	return 0;
}

