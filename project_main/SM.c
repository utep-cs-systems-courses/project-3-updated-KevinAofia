#include <msp430.h>
#include "SM.h"
#include "buzzer.h"
//Variables below are defined in our header but re-declared and 
//available so that we can change/update keys parts to our
//program if need be

char siren_state = 0;
int frequency = 700;

#define LED_GREEN BIT6
#define LED_RED BIT0

void siren() { 
  switch(siren_state)
    {
    case 0:
      buzzer_set_period(frequency);
      P1OUT |= LED_GREEN;        
      frequency = frequency + 50;
      if(frequency == 1600) {
	siren_state = 1;
	P1OUT &= ~LED_GREEN;    //turn green off
      }
      break;
    case 1:
      buzzer_set_period(frequency);
      P1OUT |= LED_RED;
      frequency = frequency - 50;
      if(frequency == 700) {
	siren_state = 0;
	P1OUT &= ~LED_RED;     //turn red off
      }
      break;
    }
}
