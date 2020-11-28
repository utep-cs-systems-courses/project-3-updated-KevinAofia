#include <msp430.h>
#include "SM.h"
#include "led.h"      //include so we can update our leds
#include "buzzer.h"   //included so we can update our frequency/buzzer 
#include "switches.h" //include so we can update our states/switches

int switch_state;     //defining our extern variable

void
__interrupt_vec(WDT_VECTOR) WDT(){	//250 interrupts/sec
  static char count = 0;//count is static so we can access variable for duration of our program
  if(switch_state != 0) { //if we are in a state(if we are pressing a switch), execute
    switch(switch_state)
      {
      case 1://switch 1
	if(++count == 50) { //every 1/5 seconds this will execute
	  blinkbuzz();
	  count = 0;
	}
	break;
      case 2://switch 2
	if(++count == 125) { //every 1/2 seconds this will execute
	  turn_on_all();
	  buzzer_set_period(1500);
	  count = 0;
	}
	break;
      case 3://switch 3
	buzzer_set_period(0);
        
	if(++count<62) {
	  dim25();
	}
	else if(++count<125) {
	  dim50();
	}
	else {
	  if(++count == 250){
	    count = 0;
	  }
	  else {
	    dim75();
	  }
	}
	break;
      case 4://switch 4
	if(++count == 5) { //every 1/50 second this will execute
	  siren();
	  count = 0;
	}
	break;
      }
  }
  else {                   //if we are not pressing a switch
    count = 0;             //ensure our interrupt is reset
    buzzer_set_period(0);  //buzzer stays off while we are not in a switch state
  }
}
