#include "System.h"
#include "utils.h"

int main(void)
{
	System system = System_create();

	bool direction = true;
	int light = 0;
	while (true)
	{
		_delay_ms(100);
		if (light == 0) ShiftRegister_write_n(&system.led_strip, true, false, false);
		if (light == 1) ShiftRegister_write_n(&system.led_strip, false, true, false);
		if (light == 2) ShiftRegister_write_n(&system.led_strip, false, false, true);
		
		light += direction * 2 - 1;

		if (light == 3 || light == -1) {
			direction = !direction;
			light += 2 * (direction * 2 - 1);
		}
	}
}