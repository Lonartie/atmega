#ifndef USART_EVENT_H
#define USART_EVENT_H

#include "../Models/ShiftRegister.h"

typedef struct USARTEvent {

} USARTEvent;

USARTEvent USARTEvent_create(ShiftRegister led);

#endif // USART_EVENT_H