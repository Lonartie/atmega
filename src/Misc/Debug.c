#include "Debug.h"
#include "../Models/USART.h"
#include "utils.h"

void debug(const char* str MAYBE_UNUSED)
{
  #if 1

  static USART usart;
  static bool initialized = false;

  if (!initialized)
  {
    usart = USART_create();
    initialized = true;
  }

  USART_send_str(&usart, str);

  #endif
}