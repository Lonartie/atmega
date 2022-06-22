#include "iesusart.h"  //defines F_CPU
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>

/** This is an example how float-numbers can be sent via the
 *  serial port. Make sure to compile this with the following
 *  options:
 *
 *  -Wl,-u,vfprintf -lprintf_flt  -lm
 * 
 *  (Extend your compile-script or your make-file accordingly!)
 */

int main(int argc, char* argv[]) {  
  USART_init(UBRR_SETTING);

  unsigned char cnt = 0;
    
  char msg[] = "pi * %d/4 = %5.3f\n";
  char output[sizeof(msg)/sizeof(msg[0]) + 20]; // Remember to have
                                                // enough mem for
                                                // numbers!
                                                // -> Count format
                                                // symbols!
        
  while (1) {
    unsigned char fac = cnt % 4 + 1;
    sprintf(output, msg, fac, atan(1)*fac);
    USART_print(output);
    cnt++;
  }

    return 0;    
}
