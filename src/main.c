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
#include <avr/io.h>


// WTF. Why is there no header here? Always seperate declaration,
// documentation, and implementation! Clean this up if you use this!

#define DR_ADC0 DDRC
#define DP_ADC0 DDC0
#define IR_ADC0 PC0
#define IP_ADC0 PC0

#define DR_ADC1 DDRC
#define DP_ADC1 DDC1
#define IR_ADC1 PC1
#define IP_ADC1 PC1

#define DR_ADC2 DDRC
#define DP_ADC2 DDC2
#define IR_ADC2 PC2
#define IP_ADC2 PC2

// The following constants reflect table 23-4 of the ATMega328p-data-
// sheet. These constants
#define ADMUX_CHN_ADC0 0 // 0000 0000
#define ADMUX_CHN_ADC1 1 // 0000 0001
#define ADMUX_CHN_ADC2 2 // 0000 0010
#define ADMUX_CHN_ADC3 3 // 0000 0011
#define ADMUX_CHN_ALL 7 // 0000 1111

// Average some measurements to reduce probable noise.
#define ADC_AVG_WINDOW 20

// Well hmmm understand this by yourself. Become inspired.
#define ADCMSG "ADC0: %5u\tADC1: %5u\tADC2: %5u\n"

const int SPEED_DRIVE_SLOW = 0;
const int SPEED_DRIVE = 200;
const int SPEED_TURN = 170; 
const int MEASURE_THRESHOLD_LEFT = 330;
const int MEASURE_THRESHOLD_MID = 400;
const int MEASURE_THRESHOLD_RIGHT = 330;

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

static int lastTimeMS = 0;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX_3(a, b, c) MAX(MAX(a, b), c)
#define IS_MAX()

void update(System* atmega)
{
	static bool left_fallback = false, right_fallback = false;

	Motor* mleft = &atmega->mt_left;
	Motor* mright = &atmega->mt_right;

	int left_measure = ADCPin_read_avg(&atmega->lf_left, 10);
	int mid_measure = ADCPin_read_avg(&atmega->lf_middle, 10);
	int right_measure = ADCPin_read_avg(&atmega->lf_right, 10);

	if (lastTimeMS + 100ul < millis()) {
		lastTimeMS = millis();
		debug(FMT("adc: %d %d %d\n", left_measure, mid_measure, right_measure));
	}

	/*bool left = left_measure > mid_measure && left_measure > right_measure; 
	bool mid = mid_measure > left_measure && mid_measure > right_measure;
	bool right = right_measure > left_measure && right_measure > mid_measure;*/

	bool left = left_measure > MEASURE_THRESHOLD_LEFT;
	bool mid = mid_measure > MEASURE_THRESHOLD_MID;
	bool right = right_measure > MEASURE_THRESHOLD_RIGHT;

	// // nothing has changed
	// if (lleft == left && lmid == mid && lright == right)
	// 	return;

	// if (left) debug("reading left\n");
	// if (mid) debug("reading mid\n");
	// if (right) debug("reading right\n");
	ShiftRegister_write_n(&atmega->led_strip, 3, left, mid, right);

	if (mid)
	{
		left_fallback = false;
		right_fallback = false;

		// only mid sensor -> move forward
		Motor_drive_forward(mleft, SPEED_DRIVE);
		Motor_drive_forward(mright, SPEED_DRIVE);

		if (left) {
			left_fallback = true;
		}

		if (right) {
			right_fallback = true;
		}
	} 
	else if (left)
	{
		// left sensor -> steer left -> move right forward
		Motor_drive_backward(mleft, SPEED_TURN);
		Motor_drive_forward(mright, SPEED_TURN);
	} 
	else if (right)
	{
		// right sensor -> steer right -> move left forward
		Motor_drive_forward(mleft, SPEED_TURN);
		Motor_drive_backward(mright, SPEED_TURN);
	} else {
		if (left_fallback) {
			// left sensor -> steer left -> move right forward
			Motor_drive_backward(mleft, SPEED_TURN);
			Motor_drive_forward(mright, SPEED_TURN);
		}
		if (right_fallback) {
			// right sensor -> steer right -> move left forward
			Motor_drive_forward(mleft, SPEED_TURN);
			Motor_drive_backward(mright, SPEED_TURN);
		}
	}
}