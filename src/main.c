#include "Models/ShiftRegister.h"
#include "utils.h"

int main(void)
{
	ShiftRegister sr = ShiftRegister_create(&DDRD, &PORTD, DDD4, &DDRB, &PORTB, DDB2, 3, 1);
	ShiftRegister_clear(&sr);

	bool direction = true;
	int light = 0;
	while (1)
	{
		_delay_ms(100);

		if (light == 0) ShiftRegister_write_n(&sr, true, false, false);
		if (light == 1) ShiftRegister_write_n(&sr, false, true, false);
		if (light == 2) ShiftRegister_write_n(&sr, false, false, true);
		
		if (direction) 	light++;
		else 						light--; 

		if (light == 3) {
			direction = !direction;
			light = 1;
		}

		if (light == -1) {
			direction = !direction;
			light = 1;
		}
	}
}