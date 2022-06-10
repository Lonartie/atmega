/*___________________________________________________________________________________________________

Title:
	micros.h v1.0

Description:
	Library code for AVR microcontrollers that triggers an interrupt every 100 microseconds
	and counts them
	
	For complete details visit
	https://www.programming-electronics-diy.xyz/2021/01/millis-and-micros-library-for-avr.html

Author:
 	Liviu Istrate
	istrateliviu24@yahoo.com
	www.programming-electronics-diy.xyz

Donate:
	Software development takes time and effort so if you find this useful consider a small donation at:
	paypal.me/alientransducer
_____________________________________________________________________________________________________*/


/* ----------------------------- LICENSE - GNU GPL v3 -----------------------------------------------

* This license must be included in any redistribution.

* Copyright (C) 2021 Liviu Istrate, www.programming-electronics-diy.xyz (istrateliviu24@yahoo.com)

* Project URL: https://www.programming-electronics-diy.xyz/2021/01/millis-and-micros-library-for-avr.html

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.

--------------------------------- END OF LICENSE --------------------------------------------------*/

#ifndef MICROS_H_
#define MICROS_H_

/*************************************************************
	INCLUDES
**************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/atomic.h>

/*************************************************************
	USER SETUP SECTION
**************************************************************/

/*
* Microseconds data type and how long it takes to overflow
* Data type				- Max time span			- Memory used
* unsigned char			- 255 microseconds		- 1 byte
* unsigned int			- 65.5 milliseconds		- 2 bytes
* unsigned long			- 71.58 minutes			- 4 bytes
* unsigned long long	- 584942 years		 	- 8 bytes
*/
typedef unsigned long micros_t;

#define MICROS_TIMER0 		0 // Use timer0
#define MICROS_TIMER1 		1 // Use timer1
#define MICROS_TIMER2 		2 // Use timer2

#define MICROS_TIMER 		MICROS_TIMER2 // Which timer to use

/*************************************************************
	--END OF USER SETUP SECTION
**************************************************************/

/*************************************************************
	SYSTEM SETTINGS
**************************************************************/

#define MICROS_RESOLUTION			100 // microseconds

#if F_CPU < 8000000
	#warning "CPU clock too low"
#endif

#if MICROS_TIMER == MICROS_TIMER0

	// Timer0
	
	#if F_CPU >= 4000000 // 4MHz - 20MHz
		#define CLOCKSEL 		(_BV(CS01))
		#define PRESCALER 		8
		
	#elif F_CPU >= 125000 // 125KHz - 4MHz
		#define CLOCKSEL 		(_BV(CS00))
		#define PRESCALER 		1
		
	#else
		#error "F_CPU not supported. Please request support."
	#endif

	#define REG_TCCRA			TCCR0A
	#define REG_TCCRB			TCCR0B
	#define REG_TIMSK			TIMSK0
	#define REG_OCR				OCR0A
	#define BIT_WGM				WGM01
	#define BIT_OCIE			OCIE0A
	#define ISR_VECT			TIMER0_COMPA_vect
	#define pwr_enable()		power_timer0_enable()
	#define pwr_disable()		power_timer0_disable()

	#define SET_TCCRA()			(REG_TCCRA = _BV(BIT_WGM))
	#define SET_TCCRB()			(REG_TCCRB = CLOCKSEL)

#elif MICROS_TIMER == MICROS_TIMER1

	// Timer1

	#if F_CPU >= 4000000 // 4MHz - 20MHz
		#define CLOCKSEL 		(_BV(CS11))
		#define PRESCALER 		8
		
	#elif F_CPU >= 125000 // 125KHz - 4MHz
		#define CLOCKSEL 		(_BV(CS10))
		#define PRESCALER 		1
		
	#else
		#error "F_CPU not supported. Please request support."
	#endif

	#define REG_TCCRA			TCCR1A
	#define REG_TCCRB			TCCR1B
	#define REG_TIMSK			TIMSK1
	#define REG_OCR				OCR1A
	#define BIT_WGM				WGM12
	#define BIT_OCIE			OCIE1A
	#define ISR_VECT			TIMER1_COMPA_vect
	#define pwr_enable()		power_timer1_enable()
	#define pwr_disable()		power_timer1_disable()

	#define SET_TCCRA()			(REG_TCCRA = 0)
	#define SET_TCCRB()			(REG_TCCRB = _BV(BIT_WGM) | CLOCKSEL)

#elif MICROS_TIMER == MICROS_TIMER2

	// Timer2

	#if F_CPU >= 4000000 // 4MHz - 20MHz
		#define CLOCKSEL 		(_BV(CS21))
		#define PRESCALER 		8
		
	#elif F_CPU >= 125000 // 125KHz - 4MHz
		#define CLOCKSEL 		(_BV(CS20))
		#define PRESCALER 		1
		
	#else
		#error "F_CPU not supported. Please request support."
	#endif

	#define REG_TCCRA			TCCR2A
	#define REG_TCCRB			TCCR2B
	#define REG_TIMSK			TIMSK2
	#define REG_OCR				OCR2A
	#define BIT_WGM				WGM21
	#define BIT_OCIE			OCIE2A
	#define ISR_VECT			TIMER2_COMPA_vect
	#define pwr_enable()		power_timer2_enable()
	#define pwr_disable()		power_timer2_disable()

	#define SET_TCCRA()			(REG_TCCRA = _BV(BIT_WGM))
	#define SET_TCCRB()			(REG_TCCRB = CLOCKSEL)

#else
	#error "Bad MICROS_TIMER set"
#endif


/*************************************************************
	GLOBAL VARIABLES
**************************************************************/
static volatile micros_t microseconds;


/*************************************************************
	FUNCTION PROTOTYPES
**************************************************************/
void micros_init(void);
micros_t micros_get(void);
void micros_resume(void);
void micros_pause(void);
void micros_reset(void);
void micros_add(micros_t us);
void micros_subtract(micros_t us);


/*************************************************************
	FUNCTIONS
**************************************************************/

// Initialise library
void micros_init(){
	// Timer settings
	SET_TCCRA();
	SET_TCCRB();
	REG_TIMSK = _BV(BIT_OCIE);
	REG_OCR = ((F_CPU / PRESCALER) / 10000) - 1; // 10000 is the frequency (1 / 10000) * 1000000 = (100 uS) 
	sei();
}

// Get current microseconds
micros_t micros_get(){
	micros_t us;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		us = microseconds;
	}
	
	return us;
}

// Turn on timer and resume time keeping
void micros_resume(){
	pwr_enable();
	REG_TIMSK |= _BV(BIT_OCIE);
}

// Pause time keeping and turn off timer to save power
void micros_pause(){
	REG_TIMSK &= ~_BV(BIT_OCIE);
	pwr_disable();
}

// Reset microseconds count to 0
void micros_reset(){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		microseconds = 0;
	}
}

// Add time
void micros_add(micros_t us){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		microseconds += us;
	}
}

// Subtract time
void micros_subtract(micros_t us){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		microseconds -= us;
	}
}


/*************************************************************
	ISR Handlers
**************************************************************/

ISR(ISR_VECT){
	microseconds += MICROS_RESOLUTION;
}


#endif /* MICROS_H_ */