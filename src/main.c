#include "Misc/utils.h"
#include "Misc/Debug.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/Timer.h"
#include "EventSystem/PhasedTimer.h"
#include "EventSystem/HardwareTimer.h"
#include "EventSystem/AnySensorWatcher.h"
#include "Models/USART.h"
#include "System.h"
#include <stdio.h>

bool visible = false;

void update(void* t);
void blink(void* t);
void log_states(void* t);
void message(bool a, bool b, bool c);

int main()
{
	timer_init();
	System atmega = System_create();

	AnySensorWatcher watcher = AnySensorWatcher_create("update", 3,
		atmega.lf_left, atmega.lf_middle, atmega.lf_right);
	PhasedTimer blink_timer = PhasedTimer_create("blink", 2, 250, 750);
	// Timer blink_timer = Timer_create(500, "blink");

	EventSystem* evq = EventSystem_instance();
	ACTOR_SCOPE(*evq) 
	{
		evq->reg_listener(Listener_create_r(&atmega, update, watcher.event));
		evq->reg_listener(Listener_create_r(&atmega, log_states, watcher.event));
		evq->reg_listener(Listener_create_r(&atmega, blink, blink_timer.event));
		ACTOR(watcher).start();
		ACTOR(blink_timer).start();
		evq->run();
	}
}

void blink(void* t)
{
	visible = !visible;
	update(t);
}

void log_states(void* t)
{
	System* atmega = (System*) t;
	bool left, middle, right;
	
	ACTOR_SCOPE(atmega->lf_left) left = atmega->lf_left.read();
	ACTOR_SCOPE(atmega->lf_middle) middle = atmega->lf_middle.read();
	ACTOR_SCOPE(atmega->lf_right) right = atmega->lf_right.read();

	message(left, middle, right);
}

void update(void* t)
{
	System* atmega = (System*) t;
	bool left, middle, right;

	ACTOR_SCOPE(atmega->lf_left) left = atmega->lf_left.read();
	ACTOR_SCOPE(atmega->lf_middle) middle = atmega->lf_middle.read();
	ACTOR_SCOPE(atmega->lf_right) right = atmega->lf_right.read();

	ACTOR(atmega->led_strip).write_n(3, 
		right && visible, 
		middle && visible, 
		left && visible);		
}

void print(char i, bool on)
{
	debug(FMT("sensor %c is now %s\n", i, on ? "on" : "off"));
}

void message(bool a, bool b, bool c)
{
	static bool _a = 0, _b = 0, _c = 0;

	if (_a != a) _a = (print('l', a), a);
	if (_b != b) _b = (print('m', b), b);
	if (_c != c) _c = (print('r', c), c);
}