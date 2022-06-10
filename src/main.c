#include "System.h"
#include "EventSystem/EventQueue.h"
#include "utils.h"

#include <stdio.h>


System atmega;

void led(uint32_t argc, void** argv)
{
	ShiftRegister_write(&atmega.led_strip, true);
}

int main()
{
	atmega = System_create();

	ShiftRegister_write(&atmega.led_strip, true);

	EventQueue_register_callback("led", led);
	Vector_bool args = Vector_bool_create();
	Vector_bool_push_back(&args, true);
	Vector_bool_push_back(&args, false);
	Vector_bool_push_back(&args, false);
	EventQueue_send_event("led", args.size, (void**)&args.data);
	Vector_bool_destroy(&args);
}

int main_2(void)
{
	bool direction = true;
	int light = 0;
	while (true)
	{
		_delay_ms(100);
		if (light == 0) ShiftRegister_write_n(&atmega.led_strip, 3, true, false, false);
		if (light == 1) ShiftRegister_write_n(&atmega.led_strip, 3, false, true, false);
		if (light == 2) ShiftRegister_write_n(&atmega.led_strip, 3, false, false, true);
		
		light += direction * 2 - 1;

		if (light == 3 || light == -1) {
			direction = !direction;
			light += 2 * (direction * 2 - 1);
		}
	}
}