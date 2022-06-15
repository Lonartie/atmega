#include "Misc/utils.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/SensorWatcher.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/PWM.h"
#include "System.h"
#include <stdio.h>
#include <util/delay.h>

void middle(void* t);

int main()
{
	timer_init();
	PWM_init();

	EventSystem* system = EventSystem_instance();
	System atmega = System_create();

	SensorWatcher watcher = SensorWatcher_create(atmega.lf_middle, "middle");

	ACTOR_SCOPE(*system)
	{
		system->reg_listener(Listener_create_r(&atmega, middle, watcher.event));
		ACTOR(watcher).start();
		system->run();
	}
}

void middle(void* t)
{
	static bool drive = false;

	drive = !drive;
	System* atmega = (System*) t;
	if (drive)
	{
		ACTOR(atmega->mt_left).set_speed(155);
		ACTOR(atmega->mt_right).set_speed(155);
		ACTOR(atmega->mt_left).drive_forward();
		ACTOR(atmega->mt_right).drive_forward();
	}
	else
	{
		ACTOR(atmega->mt_left).stop();
		ACTOR(atmega->mt_right).stop();
	}
}