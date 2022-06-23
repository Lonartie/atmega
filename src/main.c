#include "Misc/utils.h"
#include "Misc/Debug.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/AnySensorWatcher.h"
#include "EventSystem/HardwareTimer.h"
#include "EventSystem/USARTEvent.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HPTimer.h"
#include "Models/PWM.h"
#include "System.h"
#include "Menu.h"



void update(void* t);
void input(void* t);

int main()
{
	timer_init();
	PWM_init();
	Menu_init("car_start", "car_stop");

	// EventSystem* system = EventSystem_instance();
	System atmega = System_create();
	USARTEvent usart = USARTEvent_create("input");
	Timer timer = Timer_create(0, "update");

	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&atmega, System_drive, timer.event));
	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&usart, Menu_handle_input, usart.event));
	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&atmega, System_start, Menu_instance().car_start_event));
	EventSystem_reg_listener(EventSystem_instance(), Listener_create_r(&atmega, System_stop, Menu_instance().car_stop_event));

	Menu_show(Menu_instance().current);

	Timer_start(&timer);
	USARTEvent_start(&usart);

	EventSystem_run(EventSystem_instance());
}

void input(void* t)
{
	USARTEvent* usart = (USARTEvent*) t;
	debug(FMT("Got: %s", usart->data));
}
