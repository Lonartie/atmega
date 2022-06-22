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

const int SPEED_FAST = 100;
const int SPEED_SLOW = 50; 

void update(System* t);

int main()
{
	timer_init();
	PWM_init();


	// EventSystem* system = EventSystem_instance();
	System atmega = System_create();
	while (true) {
		update(&atmega);
	}


	// Timer timer = Timer_create(100, "update");
	// // AnySensorWatcher timer = AnySensorWatcher_create("update", 3, atmega.lf_left, atmega.lf_middle, atmega.lf_right);

	// EventSystem_reg_listener(system, Listener_create_r(&atmega, update, timer.event));
	// Timer_start(&timer);
	// EventSystem_run(system);
}

typedef enum State
{
	idle,
	slowly_forward,
	turn_left,
	turn_right,
	forward
} State;

void update(System* atmega)
{
	static bool lleft = false, lmid = false, lright = false;
	static State state = idle;

	Motor* mleft = &atmega->mt_left;
	Motor* mright = &atmega->mt_right;

	bool left = Pin_read(&atmega->lf_left);
	bool mid = Pin_read(&atmega->lf_middle);
	bool right = Pin_read(&atmega->lf_right);


	// // nothing has changed
	if (lleft == left && lmid == mid && lright == right)
		return;

	if (left) debug("reading left\n");
	if (mid) debug("reading mid\n");
	if (right) debug("reading right\n");
	ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);

	if (left && right)
	{
		// weird situation, just drive forward slowly
		Motor_drive_forward(&mleft, SPEED_SLOW);
		Motor_drive_forward(&mright, SPEED_SLOW);

		debug("sloooowly forward\n");
		state = slowly_forward;
	}
 	else if (mid)
	{
		// only mid sensor -> move forward
		Motor_drive_forward(mleft, SPEED_FAST);
		Motor_drive_forward(mright, SPEED_FAST);
	} 
	else if (left)
	{
		// left sensor -> steer left -> move right forward
		Motor_stop(mleft);
		Motor_drive_forward(mright, SPEED_SLOW);
	} 
	else if (right)
	{
		// right sensor -> steer right -> move left forward
		Motor_drive_forward(mleft, SPEED_SLOW);
		Motor_stop(mright);
	}
	// else if (state == slowly_forward)
	// {
	// 	Motor_stop(&mleft);
	// 	Motor_stop(&mright);
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