#include "EventSystem/EventSystem.h"
#include "EventSystem/HardwareTimer.h"
#include "EventSystem/Timer.h"
#include "Misc/Utils.h"
#include "Models/PWM.h"
#include "Models/System.h"
#include "Models/USART.h"
#include "Models/WatchDog.h"
#include "PresentationLogic.h"

int MAIN() {
  // initializes globals
  stop_watchdog();  // if resetted through watchdog, disable immediately
  timer_init();
  PWM_init();

  // create system and event senders
  System atmega = System_create();
  USART* usart = USART_instance();
  Timer timer = Timer_create(0, "update");

  presentation_start(&atmega);

  // register event listeners
  EventSystem_reg_listener(
      EventSystem_instance(),
      Listener_create_r(&atmega, presentation_update, timer.event));

  EventSystem_reg_listener(
      EventSystem_instance(),
      Listener_create_r(usart, presentation_handle_command, usart->event));

  // start updaters
  Timer_start(&timer);
  USART_start(usart);

  // run event loop
  EventSystem_run(EventSystem_instance());

  return 0;
}
