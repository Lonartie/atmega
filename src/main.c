#include "Misc/utils.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/AnySensorWatcher.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/PWM.h"
#include "System.h"
#include <stdio.h>
#include <util/delay.h>

void update(void* t);

int main()
{
	timer_init();
	PWM_init();

	EventSystem* system = EventSystem_instance();
	System atmega = System_create();

	AnySensorWatcher watcher = AnySensorWatcher_create("update", 3, atmega.lf_left, atmega.lf_middle, atmega.lf_right);

	ACTOR_SCOPE(*system)
	{
		system->reg_listener(Listener_create_r(&atmega, update, watcher.event));
		ACTOR(watcher).start();
		system->run();
	}
}

void update(void* t)
{
	static bool last_left = false;
	static bool last_mid = false;
	static bool last_right = false;
	static bool keep_mid = false;
	System* atmega = (System*) t;
	Motor mleft = atmega->mt_left;
	Motor mright = atmega->mt_right;
	keep_mid = false;

	bool left, mid, right;
	ACTOR_SCOPE(atmega->lf_left) left = atmega->lf_left.read();
	ACTOR_SCOPE(atmega->lf_middle) mid = atmega->lf_middle.read();
	ACTOR_SCOPE(atmega->lf_right) right = atmega->lf_right.read();

	if (mid || last_mid)
	{
		ACTOR(mleft).set_speed(130);
		ACTOR(mright).set_speed(130);
		ACTOR(mleft).drive_forward();
		ACTOR(mright).drive_forward();
	}

	if (left)
	{
		ACTOR(mleft).set_speed(30);
		ACTOR(mright).set_speed(80);
		ACTOR(mleft).drive_forward();
		ACTOR(mright).drive_forward();
	}

	if (right)
	{
		ACTOR(mleft).set_speed(80);
		ACTOR(mright).set_speed(30);
		ACTOR(mleft).drive_forward();
		ACTOR(mright).drive_forward();
	}

	if (!left && !mid && !right && !last_mid)
	{
		ACTOR(mleft).stop();
		ACTOR(mright).stop();
	}

	if (!left && !right && !mid && last_mid)
	{
		keep_mid = true;
	}

	last_left = left;
	if (!keep_mid) last_mid = mid;
	last_right = right;
}