#include "Misc/utils.h"
#include "EventSystem/EventQueue.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/USART.h"
#include "System.h"

System atmega;
Timer timer;
USART usart;

void main_2(int, void*);
int main()
{
	timer_init();
	atmega = System_create();
	usart = USART_create();
	timer = Timer_create(100, "main_timer_100_ms");

	EventQueue* evq = EventQueue_instance();
	ACTOR_SCOPE(*evq) 
	{
		evq->reg_listener(Listener_create(main_2, "main_timer_100_ms"));
		ACTOR(timer).start();	
		evq->run();
	}
}

void main_2(int argc MAYBE_UNUSED, void* argv MAYBE_UNUSED)
{
	static int light = 0;
	static bool direction = true;

	ACTOR_SCOPE(atmega.led_strip)
	{
		if (light == 0) atmega.led_strip.write_n(3, true, false, false);
		if (light == 1) atmega.led_strip.write_n(3, false, true, false);
		if (light == 2) atmega.led_strip.write_n(3, false, false, true);
	}

	ACTOR_SCOPE(usart)
	{
		if (light == 0) usart.send_str("Left");
		if (light == 1) usart.send_str("Middle");
		if (light == 2) usart.send_str("Right");
	}
	
	light += direction * 2 - 1;
	if (light == 3 || light == -1) {
		direction = !direction;
		light += 2 * (direction * 2 - 1);
	}
}