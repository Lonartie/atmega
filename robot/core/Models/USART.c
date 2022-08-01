#define BAUD 9600
#define UBRR_SETTING F_CPU / 16.0 / BAUD - 1

#include "USART.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

#include "EventSystem/EventSystem.h"
#include "Misc/Utils.h"

static USART instance;
static bool initialized = false;
static char* volatile data;
static volatile bool ready_read = false;

USART* USART_instance() {
  if (!initialized) {
    instance.ubrr = UBRR_SETTING;

    if (!initialized) {
      data = NULL;
      initialized = true;
    }

    instance.event = USART_EVENT_NAME;
    instance.data = NULL;

    UBRR0H = (unsigned char)(instance.ubrr >> 8);
    UBRR0L = (unsigned char)instance.ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    initialized = true;
  }

  return &instance;
}

void USART_send_byte(USART* usart MAYBE_UNUSED, char byte) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = byte;
}

void USART_send_str(USART* usart, const char* str) {
  while (*str != '\0') {
    USART_send_byte(usart, *str);
    str++;
  }
}

char USART_recv_byte(USART* usart MAYBE_UNUSED) {
  while (!(UCSR0A & (1 << RXC0)))
    ;
  return UDR0;
}

void USART_start(USART* _this) {
  EventSystem_reg_updater(EventSystem_instance(),
                          Updater_create(_this, USART_update));
}

void USART_stop(USART* _this) {
  EventSystem_unreg_updater(EventSystem_instance(),
                            Updater_create(_this, USART_update));
}
void free_data() {
  if (data != NULL) {
    free(data);
    data = NULL;
  }
}

void USART_update(void* _this) {
  USART* usart = (USART*)_this;
  if (ready_read && data != NULL) {
    // send event
    ready_read = false;
    usart->data = data;
    EventSystem_send_event(EventSystem_instance(), Event_create(usart->event));

    // after event system has handled the event, free the data
    usart->data = NULL;
    free_data();
  }
}

ISR(USART_RX_vect) {
  free_data();
  data = (char*)malloc(sizeof(char) * 1);
  uint8_t i = 0;

  // get the whole message
  while (1) {
    data[i] = USART_recv_byte(USART_instance());
    if (data[i] == '\n') {
      break;
    }
    data = (char*)realloc(data, sizeof(char) * (i + 1));
    ++i;
  }

  // mark as ready
  data[i] = '\0';
  ready_read = true;
}