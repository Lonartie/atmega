#include "Misc/utils.h"
#include "Misc/Debug.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/AnySensorWatcher.h"
#include "EventSystem/HardwareTimer.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HPTimer.h"
#include "Models/PWM.h"
#include "System.h"
#include <stdio.h>
#include <util/delay.h>

void update(void* t);

int main()
{
	timer_init();
	PWM_init();

	System atmega = System_create();
	
	Motor mleft = atmega.mt_left;
	Motor mright = atmega.mt_right;

	
	ACTOR(mleft).drive_forward(30);
	ACTOR(mright).drive_forward(30);

	_delay_ms(3000);

	ACTOR(mleft).stop();
	ACTOR(mright).stop();


	// Timer timer = Timer_create(10, "update");
	// // AnySensorWatcher timer = AnySensorWatcher_create("update", 3, atmega.lf_left, atmega.lf_middle, atmega.lf_right);

	// ACTOR_SCOPE(*system)
	// {
	// 	system->reg_listener(Listener_create_r(&atmega, update, timer.event));
	// 	ACTOR(timer).start();
	// 	system->run();
	// }
}

typedef enum State
{
	idle,
	slowly_forward,
	turn_left,
	turn_right,
	forward
} State;

void update(void* t)
{
	static bool lleft = false, lmid = false, lright = false;
	static State state = idle;
	static uint64_t time = 0;

	System* atmega = (System*) t;
	Motor mleft = atmega->mt_left;
	Motor mright = atmega->mt_right;

	bool left, mid, right;

	ACTOR_SCOPE(atmega->lf_left) left = atmega->lf_left.read();
	ACTOR_SCOPE(atmega->lf_middle) mid = atmega->lf_middle.read();
	ACTOR_SCOPE(atmega->lf_right) right = atmega->lf_right.read();

	ACTOR(mleft).drive_forward(30);
	ACTOR(mright).drive_forward(30);
	if (millis() == 3000)
	{
		ACTOR(mleft).stop();
		ACTOR(mright).stop();
	}

	// // nothing has changed
	// if (lleft == left && lmid == mid && lright == right)
	// 	return;

	// if (left && right)
	// {
	// 	// weird situation, just drive forward slowly
	// 	ACTOR(mleft).drive_forward(30);
	// 	ACTOR(mright).drive_forward(30);

	// 	debug("sloooowly forward\n");
	// 	state = slowly_forward;

	// 	ACTOR(atmega->led_strip).write_n(3, 1, 0, 1);
	// }
 	// else if (right)
	// {
	// 	// right sensor -> steer right -> move left forward
	// 	ACTOR(mleft).drive_forward(15);
	// 	ACTOR(mright).stop();

	// 	debug("steer right\n");
	// 	state = turn_right;

	// 	ACTOR(atmega->led_strip).write_n(3, 0, 0, 1);
	// }
	// else if (left)
	// {
	// 	// left sensor -> steer left -> move right forward
	// 	ACTOR(mleft).stop();
	// 	ACTOR(mright).drive_forward(15);

	// 	debug("steer left\n");
	// 	state = turn_left;

	// 	ACTOR(atmega->led_strip).write_n(3, 1, 0, 0);
	// }
	// else if (mid)
	// {
	// 	// only mid sensor -> move forward
	// 	ACTOR(mleft).drive_forward(100);
	// 	ACTOR(mright).drive_forward(100);

	// 	debug("forward\n");
	// 	state = forward;

	// 	ACTOR(atmega->led_strip).write_n(3, 0, 1, 0);
	// }
	// else if (state == slowly_forward)
	// {
	// 	ACTOR(mleft).stop();
	// 	ACTOR(mright).stop();
	// 	debug("stopping!\n");
	// }
	// else
	// {
	// 	debug("unknown state\n");
	// }

	lleft = left;
	lmid = mid;
	lright = right;
}