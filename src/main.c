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

	EventSystem* system = EventSystem_instance();
	System atmega = System_create();


	Timer timer = Timer_create(10, "update");
	// AnySensorWatcher timer = AnySensorWatcher_create("update", 3, atmega.lf_left, atmega.lf_middle, atmega.lf_right);

	EventSystem_reg_listener(system, Listener_create_r(&atmega, update, timer.event));
	Timer_start(&timer);
	EventSystem_run(system);
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

	System* atmega = (System*) t;
	Motor mleft = atmega->mt_left;
	Motor mright = atmega->mt_right;

	bool left = Pin_read(&atmega->lf_left);
	bool mid = Pin_read(&atmega->lf_middle);
	bool right = Pin_read(&atmega->lf_right);

	// nothing has changed
	if (lleft == left && lmid == mid && lright == right)
		return;

	if (left && right)
	{
		// weird situation, just drive forward slowly
		Motor_drive_forward(&mleft, 30);
		Motor_drive_forward(&mright, 30);

		debug("sloooowly forward\n");
		state = slowly_forward;

		ShiftRegister_write_n(&atmega->led_strip, 3, 1, 0, 1);
	}
 	else if (right)
	{
		// right sensor -> steer right -> move left forward
		Motor_drive_forward(&mleft, 15);
		Motor_stop(&mright);

		debug("steer right\n");
		state = turn_right;

		ShiftRegister_write_n(&atmega->led_strip, 3, 0, 0, 1);
	}
	else if (left)
	{
		// left sensor -> steer left -> move right forward
		Motor_stop(&mleft);
		Motor_drive_forward(&mright, 15);

		debug("steer left\n");
		state = turn_left;

		ShiftRegister_write_n(&atmega->led_strip, 3, 1, 0, 0);
	}
	else if (mid)
	{
		// only mid sensor -> move forward
		Motor_drive_forward(&mleft, 100);
		Motor_drive_forward(&mright, 100);

		debug("forward\n");
		state = forward;

		ShiftRegister_write_n(&atmega->led_strip, 3, 0, 1, 0);
	}
	else if (state == slowly_forward)
	{
		Motor_stop(&mleft);
		Motor_stop(&mright);
		debug("stopping!\n");
	}
	else
	{
		debug("unknown state\n");
	}

	lleft = left;
	lmid = mid;
	lright = right;
}