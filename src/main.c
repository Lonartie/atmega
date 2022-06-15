#include "Misc/utils.h"
#include "EventSystem/HardwareTimer.h"
#include "Models/PWM.h"
#include "System.h"
#include <stdio.h>
#include <util/delay.h>

int main()
{
	timer_init();
	PWM_init();

	System atmega = System_create();
	ACTOR(atmega.mt_left).set_speed(50);
	ACTOR(atmega.mt_right).set_speed(50);
	ACTOR(atmega.mt_left).drive_forward();
	ACTOR(atmega.mt_right).drive_backward();

	_delay_ms(1000);
	ACTOR(atmega.mt_left).stop();
	ACTOR(atmega.mt_right).stop();
}