#include "Misc/utils.h"
#include "EventSystem/EventQueue.h"
#include "EventSystem/Timer.h"
#include "EventSystem/HardwareTimer.h"
#include "System.h"

System atmega;

void main_2(void);
int main()
{
	timer_init();
	atmega = System_create();

	Timer timer = Timer_create(100, main_2);
	call(timer, start);
	
	callp(EventQueue_instance(), run);
}

void main_2(void)
{
	static int light = 0;
	static bool direction = true;

	if (light == 0) calln(atmega.led_strip, write_n, (3, true, false, false));
	if (light == 1) calln(atmega.led_strip, write_n, (3, false, true, false));
	if (light == 2) calln(atmega.led_strip, write_n, (3, false, false, true));
	
	light += direction * 2 - 1;

	if (light == 3 || light == -1) {
		direction = !direction;
		light += 2 * (direction * 2 - 1);
	}
}