#include "Misc/Utils.h"
#include "EventSystem/USARTEvent.h"
#include "EventSystem/HardwareTimer.h"
#include "EventSystem/Timer.h"
#include "EventSystem/EventSystem.h"
#include "Models/PWM.h"
#include "Models/System.h"
#include "Models/Menu.h"
#include "DrivingLogic.h"

int MAIN()
{
	// initializes globals
	timer_init();
	PWM_init();
	Menu_init("car_start", "car_stop");

	// create system and event senders
	System atmega = System_create();
	USARTEvent usart = USARTEvent_create("input");
	Timer timer = Timer_create(0, "update");

	// register event listeners
	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&atmega, System_drive, timer.event));
	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&usart, Menu_handle_input, usart.event));
	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&atmega, System_start, Menu_instance().car_start_event));
	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&atmega, System_stop, Menu_instance().car_stop_event));

	// show usart menu
	Menu_show(Menu_instance().current);

	// start event senders
	Timer_start(&timer);
	USARTEvent_start(&usart);

	// start event loop
	EventSystem_run(EventSystem_instance());
	
	return 0;
}