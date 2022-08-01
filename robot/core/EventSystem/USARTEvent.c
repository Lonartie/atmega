#include "USARTEvent.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

#include "EventSystem.h"
#include "Models/USART.h"

static bool initialized = false;
static char* volatile data;
static volatile bool ready_read = false;

USARTEvent USARTEvent_create(String event) {
  if (!initialized) {
    data = NULL;
    initialized = true;
  }

  USARTEvent usart_event;
  usart_event.event = event;
  usart_event.data = NULL;

  return usart_event;
}

void USARTEvent_start(USARTEvent* _this) {
  EventSystem_reg_updater(EventSystem_instance(),
                          Updater_create(_this, USARTEvent_update));
}

void USARTEvent_stop(USARTEvent* _this) {
  EventSystem_unreg_updater(EventSystem_instance(),
                            Updater_create(_this, USARTEvent_update));
}

void free_data() {
  if (data != NULL) {
    free(data);
    data = NULL;
  }
}

void USARTEvent_update(void* _this) {
  USARTEvent* usart_event = (USARTEvent*)_this;
  if (ready_read && data != NULL) {
    // send event
    ready_read = false;
    usart_event->data = data;
    EventSystem_send_event(EventSystem_instance(),
                           Event_create(usart_event->event));

    // after event system has handled the event, free the data
    usart_event->data = NULL;
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
    if (data[i] == '\r') {
      break;
    }
    data = (char*)realloc(data, sizeof(char) * (i + 1));
    ++i;
  }

  // mark as ready
  data[i] = '\0';
  ready_read = true;
}