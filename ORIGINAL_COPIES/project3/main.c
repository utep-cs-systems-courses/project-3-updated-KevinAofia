//Alternate LEDs from Off, Green, and Red
//initialize buzzer and switches
#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"             //merging our buzzer
#include "switches.h"           //merging our switches

int main(void) {
  configureClocks();		//setup master oscillator, CPU & peripheral clocks
  led_init();                   //init our leds
  buzzer_init();                //init our buzzer
  switch_init();                //init our switches

  enableWDTInterrupts();	//enable periodic interrupt
    
  or_sr(0x18);		        // CPU off, GIE on
}
