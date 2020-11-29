#include <msp430.h>
#include "SM.h"
#include "buzzer.h"

char siren_state = 0;
int frequency = 700;

void siren() { 
  switch(siren_state)
    {
    case 0:
      buzzer_set_period(frequency);        
      frequency = frequency + 50;
      if(frequency == 1600) {
	siren_state = 1;
      }
      break;
    case 1:
      buzzer_set_period(frequency);
      frequency = frequency - 50;
      if(frequency == 700) {
	siren_state = 0;
      }
      break;
    }
}
