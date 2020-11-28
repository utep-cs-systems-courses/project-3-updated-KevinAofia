#include <msp430.h>
#include "switches.h"
//this is our interrupt handler for our screen and switches
//Switch on P2
void
__interrupt_vec(PORT2_VECTOR) Port_2(){ //Port_2_Vector is a vector of interrupts
  if (P2IFG & SWITCHES) {       //p2IFG is a vector of flags that tell us what caused an interrrupt
    P2IFG &= ~SWITCHES;	        //clear pending sw interrupts
    switch_interrupt_handler();	//single handler for all switches 
  }
}
