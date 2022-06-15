#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "iesusart.h"

// Robot function/peripheral RIGHT LF.
#define DR_LF_R DDRC
#define DP_LF_R DDC0
#define IR_LF_R PINC
#define IP_LF_R PINC0

// Robot function/peripheral MIDDLE LF.
#define DR_LF_M DDRC
#define DP_LF_M DDC1
#define IR_LF_M PINC
#define IP_LF_M PINC1

// Robot function/peripheral LEFT LF.
#define DR_LF_L DDRC
#define DP_LF_L DDC2
#define IR_LF_L PINC
#define IP_LF_L PINC2

// Robot funktion/peripheral SR
#define REGWIDTH 3

// SR clock
#define DR_SR_CLK  DDRD
#define DP_SR_CLK  DDD4
#define OR_SR_CLK  PORTD
#define OP_SR_CLK  PORTD4

// SR data
#define DR_SR_DATA DDRB
#define DP_SR_DATA DDB2
#define OR_SR_DATA PORTB
#define OP_SR_DATA PORTB2

#define SR_LED_YELLOW 2
#define SR_LED_GREEN  1
#define SR_LED_BLUE   0

typedef unsigned char srr_t;
typedef unsigned char cntv8_t;

/**
  Clocks the real hardware -- whenever this is called,
  a rising edge on PD4 is generated.
*/
void clk() {
    OR_SR_CLK &= ~(1 << OP_SR_CLK);
    OR_SR_CLK |= (1 << OP_SR_CLK);
    // There may be dragons in here. How long has a
    // clock pulse to be high? How do you know?
    OR_SR_CLK &= ~(1 << OP_SR_CLK);
}

/**
  Writes the in-memory-representation (the "model") of
  the robot's shift-register to the real hardware.
*/
void update_hardware(srr_t *regmdl) {
    for(cntv8_t i = 0; i < REGWIDTH; i++) {
        unsigned char position_set = (*regmdl & (1 << i));
        if (position_set) {
           OR_SR_DATA |= (1 << OP_SR_DATA);
           switch (position_set) {
              case (1 << SR_LED_BLUE):
                 USART_print("LEFT LED SET/LD FIRES!\n\n");
                 break;
              case (1 << SR_LED_GREEN):
                 USART_print("MIDDLE LED SET/LF FIRES!\n\n");
                 break;
              case (1 << SR_LED_YELLOW):
                 USART_print("RIGHT LED SET/LF FIRES!\n\n");
                 break;
              default:
                 USART_print("NO OR MULTIPLE LFs FIRE!\n\n");
           }
        } else {
           OR_SR_DATA &= ~(1 << OP_SR_DATA);
        }
        clk();
    }
}

/**
  Updates the model (memory), that represents the
  robot's shift-register.
*/
void update_model(srr_t *regmdl) {

    if (IR_LF_R & (1 << IP_LF_R)) {
        *regmdl |= (1 << SR_LED_YELLOW);
    } else {
        *regmdl &= ~(1 << SR_LED_YELLOW);
    }

    if (IR_LF_M & (1 << IP_LF_M)) {
        *regmdl |= (1 << SR_LED_GREEN);
    } else {
        *regmdl &= ~(1 << SR_LED_GREEN);
    }

    if (IR_LF_L & (1 << IP_LF_L)) {
        *regmdl |= (1 << SR_LED_BLUE);
    } else {
        *regmdl &= ~(1 << SR_LED_BLUE);
    }

}

/**
  Clears the in-memory-representation (the "model") of
  the robot's shift-register.
*/
void clear(srr_t *regmdl) {
    *regmdl = 0;
    update_hardware(regmdl);
}

/**
  Sets data directions.
*/
void setup_ddr_all() {
    // Set Data Direction Register B2 as output.
    DR_SR_DATA = 1 << DP_SR_DATA;

    // Set Data Direction Register D4 as output. To D4,
    // the CLK-line of the robot's shift-register is connected.
    DR_SR_CLK = 1 << DP_SR_CLK;

    // Set Data Direction Register C[0|1|2] as input. To these pins,
    // the robot's line-infrared-reflection-sensors are attached.
    DR_LF_L &= ~(1 << DP_LF_L);
    DR_LF_M &= ~(1 << DP_LF_R);
    DR_LF_M &= ~(1 << DP_LF_M);
}

int main(void) {
    // Setup everything
    setup_ddr_all();
    USART_init(UBRR_SETTING);

    // Allocate 1 byte in memory/on heap for a representation (model)
    // of the register and clear the contents directly, and update
    // everything accordingly.
    srr_t *regmdl = malloc(sizeof(srr_t));
    clear(regmdl);

    srr_t last_model_state = *regmdl;

    while (1) {

        update_model(regmdl);

        if (*regmdl != last_model_state) {
           update_hardware(regmdl);
           last_model_state = *regmdl;
        }

    }

    return 0;
}
