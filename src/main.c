#include "Misc/utils.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/USART.h"
#include "System.h"

typedef struct Tuple 
{
	USART usart;
	System atmega; 
} Tuple;

void main_loop(void* t);
void exit_callback(void* t);

int main()
{
	timer_init();
	Tuple tuple;
	tuple.usart = USART_create();
	tuple.atmega = System_create();
	Timer timer = Timer_create(500, "main_timer_333_ms");
	Timer exit_timer = Timer_create(10000, "exit");

	EventSystem* evq = EventSystem_instance();
	ACTOR_SCOPE(*evq) 
	{
		evq->reg_listener(Listener_create_r(&tuple, main_loop, timer.event_type));
		evq->reg_listener(Listener_create_r(&tuple, exit_callback, exit_timer.event_type));
		ACTOR(timer).start();	
		ACTOR(exit_timer).start();
		evq->run();
	}

	ACTOR(tuple.usart).send_str("All timers have been stopped.\n");
}

void main_loop(void* t)
{
	static int light = 0;
	static bool direction = true;
	Tuple* tuple = (Tuple*)t;
	System* mega = &tuple->atmega;
	USART* usart = &tuple->usart;

	ACTOR_SCOPE(mega->led_strip)
	{
		if (light == 0) mega->led_strip.write_n(3, true, false, false);
		if (light == 1) mega->led_strip.write_n(3, false, true, false);
		if (light == 2) mega->led_strip.write_n(3, false, false, true);
	}

	ACTOR_SCOPE(*usart)
	{
		if (light == 0) usart->send_str("Left\n");
		if (light == 1) usart->send_str("Middle\n");
		if (light == 2) usart->send_str("Right\n");
	}
	
	light += direction * 2 - 1;
	if (light == 3 || light == -1) {
		direction = !direction;
		light += 2 * (direction * 2 - 1);
	}
}

void exit_callback(void* t)
{
	Tuple* tuple = (Tuple*)t;
	USART* usart = &tuple->usart;

	ACTOR(*usart).send_str("about to end\n");
	ACTOR(*EventSystem_instance()).exit();
}