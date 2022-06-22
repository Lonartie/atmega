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
const int SPEED_DRIVE = 180;
const int SPEED_TURN = 200; 
const int MEASURE_THRESHOLD_LEFT = 330;
const int MEASURE_THRESHOLD_MID = 400;
const int MEASURE_THRESHOLD_RIGHT = 330;

void update(System* t);

void ADC_init(void) {
  // The following lines still let the digital input registers enabled,
  // though that's not a good idea (energy-consumption).

  // Klare Verhältnisse erstma!
  ADMUX = 0;

  // Sets AVcc as the ADC's voltage source and as the reference,
  // while that voltage stems from the AREF-pin (5V when the robots is
  //  powered by batteries, lower otherwise).
  ADMUX = (1<<REFS0);

  // This sets how fast (and: accurate) the ADC can run.
  // All these bits set to 1 set a sysclock-division of 128,
  // so the ADC can run with up to 125 kHz.
  ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

  // This enables/really turns on the ADC. 
  ADCSRA |= (1<<ADEN);

  // The following lines of code start a single measurement in single
  // conversion mode. Needed once to "warm up" the ADC. The contents
  // of the result-register ADCW are ignored because they are not
  // reliable yet (until warm-up is done). ADSC goes to 0 when the
  // conversion is done. The first conversion is not only inreliable,
  // but also 25 ADC-cycles long, while the next conversions are around
  // 13 cycles long.
  ADCSRA |= (1<<ADSC);
  while (ADCSRA & (1<<ADSC)) {
    // zzzZZZzzzZZZzzz ... take a sleep until measurement done.
  }
  ADCW;
}

uint16_t ADC_read(uint8_t channel) {
  // Remember to have the ADC initialized!

  // The following line does set all ADMUX-MUX-pins to 0, disconnects
  // all channels from the MUX.
  ADMUX &= ~(ADMUX_CHN_ALL);
  ADMUX |= channel;

  // We start a single measurement and then busy-wait until
  // the ADSC-bit goes to 0, signalling the end of the measurement.
  ADCSRA |= (1<<ADSC);
  while (ADCSRA & (1<<ADSC)) {
    // zzzZZZzzzZZZzzz ... take a sleep until measurement done.
  }
  // Again, a pointer-airthmetical expression. the ADC-register has a
  // lower and a higher portion, but
  return ADCW;
}

uint16_t ADC_read_avg(uint8_t channel, uint8_t nsamples) {
  // How large does our datatype need to be?
  uint32_t sum = 0;

  for (uint8_t i = 0; i < nsamples; ++i ) {
    sum += ADC_read(channel);
  }

  return (uint16_t)( sum / (float)nsamples );
}

int main()
{
	timer_init();
	PWM_init();

	DR_ADC0 &= ~(1 << DP_ADC0);
	DR_ADC1 &= ~(1 << DP_ADC1);
	DR_ADC2 &= ~(1 << DP_ADC2);
	ADC_init();

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
	static bool lleft = false, lmid = false, lright = false;
	static bool left_fallback = false, right_fallback = false;

	Motor* mleft = &atmega->mt_left;
	Motor* mright = &atmega->mt_right;

	int left_measure = ADC_read_avg(ADMUX_CHN_ADC2, ADC_AVG_WINDOW);
	int mid_measure = ADC_read_avg(ADMUX_CHN_ADC1, ADC_AVG_WINDOW);
	int right_measure = ADC_read_avg(ADMUX_CHN_ADC0, ADC_AVG_WINDOW);

	if (lastTimeMS + 100 < millis()) {
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

	// if (left_measure > 400) {
	// 	left_fallback = true;
	// }

	// if (right_measure > 400) {
	// 	right_fallback = true;
	// }

	lleft = left;
	lmid = mid;
	lright = right;
}