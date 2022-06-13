#include "Misc/utils.h"
#include "EventSystem/EventQueue.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/USART.h"
#include "System.h"

typedef struct Tuple { USART usart; System atmega; } Tuple;
void main_2(void* t);
int main()
{
	timer_init();
	Tuple tuple;
	tuple.usart = USART_create();
	tuple.atmega = System_create();
	Timer timer = Timer_create(333, "main_timer_333_ms");

	EventQueue* evq = EventQueue_instance();
	ACTOR_SCOPE(*evq) 
	{
		evq->reg_listener(Listener_create_with_receiver(&tuple, main_2, timer.event_type));
		ACTOR(timer).start();	
		evq->run();
	}
}

void main_2(void* t)
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