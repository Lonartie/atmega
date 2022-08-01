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

  Logic_start(&atmega);

  // register event listeners
  EventSystem_reg_listener(
      EventSystem_instance(),
      Listener_create_r(&atmega, Logic_drive_3_rounds, timer.event));

  EventSystem_reg_listener(
      EventSystem_instance(),
      Listener_create_r(usart, Logic_command, usart->event));

  //   EventSystem_reg_listener(
  //       EventSystem_instance(),
  //       Listener_create_r(&atmega, Logic_reset,
  //       Menu_instance().car_start_event));

  //   EventSystem_reg_listener(
  //       EventSystem_instance(),
  //       Listener_create_r(&atmega, Logic_reset,
  //       Menu_instance().car_stop_event));

  //   EventSystem_reg_listener(EventSystem_instance(),
  //                            Listener_create_r(&atmega, System_start,
  //                                              Menu_instance().car_start_event));

  //   EventSystem_reg_listener(
  //       EventSystem_instance(),
  //       Listener_create_r(&atmega, System_stop,
  //       Menu_instance().car_stop_event));

  //   // show usart menu
  //   Menu_show(Menu_instance().current);

  // start event senders
  Timer_start(&timer);
  USART_start(usart);

  // start event loop
  EventSystem_run(EventSystem_instance());

  return 0;
}
