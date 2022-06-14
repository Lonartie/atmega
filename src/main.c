#include "Misc/utils.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HardwareTimer.h"
#include "EventSystem/AnySensorWatcher.h"
#include "Models/USART.h"
#include "System.h"

void update(void* t);

int main()
{
	timer_init();
	System atmega = System_create();

	AnySensorWatcher watcher = AnySensorWatcher_create("update", 3,
		atmega.lf_left, atmega.lf_middle, atmega.lf_right);

	EventSystem* evq = EventSystem_instance();
	ACTOR_SCOPE(*evq) 
	{
		evq->reg_listener(Listener_create_r(&atmega, update, watcher.event));
		ACTOR(watcher).start();
		evq->run();
	}
}

void update(void* t)
{
	System* atmega = (System*) t;
	bool left, middle, right;
	
	ACTOR_SCOPE(atmega->lf_left) left = atmega->lf_left.read();
	ACTOR_SCOPE(atmega->lf_middle) middle = atmega->lf_middle.read();
	ACTOR_SCOPE(atmega->lf_right) right = atmega->lf_right.read();

	ACTOR(atmega->led_strip).write_n(3, left, middle, right);
}