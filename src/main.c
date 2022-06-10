#include "System.h"
#include "EventSystem/EventQueue.h"
#include "EventSystem/Timer.h"
#include "utils.h"

#include <stdio.h>


System atmega;

void led(uint32_t argc, void** argv)
{
	ShiftRegister_write(&atmega.led_strip, true);
}


void main_2(void);
int main()
{
	atmega = System_create();
	volatile Timer timer = Timer_create(100, main_2);
}

void main_2(void)
{
	static int light = 0;
	static bool direction = true;

	if (light == 0) ShiftRegister_write_n(&atmega.led_strip, 3, true, false, false);
	if (light == 1) ShiftRegister_write_n(&atmega.led_strip, 3, false, true, false);
	if (light == 2) ShiftRegister_write_n(&atmega.led_strip, 3, false, false, true);
	
	light += direction * 2 - 1;

	if (light == 3 || light == -1) {
		direction = !direction;
		light += 2 * (direction * 2 - 1);
	}
}