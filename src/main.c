#include "Misc/utils.h"
#include "EventSystem/EventQueue.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HardwareTimer.h"
#include "System.h"

System atmega;
Timer timer;

void main_2(void);
int main()
{
	timer_init();
	atmega = System_create();

	timer = Timer_create(100, "main_timer_100_ms");
	EventQueue_register_listener(EventQueue_instance(), Listener_create(main_2, "main_timer_100_ms"));
	ACTOR_SCOPE(timer) timer.start();	
	EventQueue_run(EventQueue_instance());
}

void main_2(void)
{
	static int light = 0;
	static bool direction = true;

	ACTOR_SCOPE(atmega)
	{
		ACTOR_SCOPE(atmega.led_strip)
		{
			if (light == 0) atmega.led_strip.write_n(3, true, false, false);
			if (light == 1) atmega.led_strip.write_n(3, false, true, false);
			if (light == 2) atmega.led_strip.write_n(3, false, false, true);
		}

		if (get_actor() != &atmega)
		{
			ShiftRegister_write_n(&atmega.led_strip, 3, true, true, true);
		}
	}
	
	light += direction * 2 - 1;

	if (light == 3 || light == -1) {
		direction = !direction;
		light += 2 * (direction * 2 - 1);
	}
}