#ifndef USART_EVENT_H
#define USART_EVENT_H

#include "../Misc/String.h"

typedef struct USARTEvent {
  String event;
  char* volatile data;
} USARTEvent;

USARTEvent USARTEvent_create(String event);
void USARTEvent_start(USARTEvent* _this);
void USARTEvent_stop(USARTEvent* _this);
void USARTEvent_update(void* _this);

#endif // USART_EVENT_H