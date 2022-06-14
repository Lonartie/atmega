#include "Debug.h"
#include "../Models/USART.h"

void debug(const char* str)
{
  static USART usart;
  static bool initialized = false;

  if (!initialized)
  {
    usart = USART_create();
    initialized = true;
  }

  USART_send_str(&usart, str);
}