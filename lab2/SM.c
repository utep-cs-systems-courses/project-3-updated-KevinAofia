#include <msp430.h>
#include "SM.h"
#include "led.h"
#include "buzzer.h"
//Variables below are defined in our header but re-declared and 
//available so that we can change/update keys parts to our
//program if need be

char blinkbuzz_state = 0;

char siren_state = 0;
int frequency = 700;

int dim_state = 0;
int dim_state2 = 0;

void turn_on_red() {
  red_on = 1;
  green_on = 0; //turns green off
  led_changed = 1;
  led_update();
}
void turn_on_green() {
  green_on = 1;
  red_on = 0; //turns red off
  led_changed = 1;
  led_update();
}
void turn_on_all() {
  red_on = 1;
  green_on = 1;
  led_changed = 1;
  led_update();
}
void blinkbuzz() {
  //static char blinkbuzz_state = 0;
  switch(blinkbuzz_state)
    {
    case 0:
      turn_on_green();
      buzzer_set_period(7632);
      blinkbuzz_state = 1;
      break;
    case 1:
      turn_on_all();
      buzzer_set_period(5714);
      blinkbuzz_state = 2;
      break;
    case 2:
      turn_on_red();
      buzzer_set_period(4048);
      blinkbuzz_state = 0;
      break;
    }
}
void siren() { 
  //static char siren_state = 0;
  //static int frequency = 700;
  switch(siren_state)
    {
    case 0:
      buzzer_set_period(frequency);
      frequency = frequency + 10;
      if(frequency == 1600) {
	siren_state = 1;
      }
      break;
    case 1:
      buzzer_set_period(frequency);
      frequency = frequency - 10;
      if(frequency == 700) {
	siren_state = 0;
      }
      break;
    }
}
void dim() {
  static int dim_counter = 0;
  if(dim_counter < 62) {
    dim25();
    dim_counter += 1;
  }
  else if(dim_counter < 125) {
    dim50();
    dim_counter += 1;
  }
  else {
    if(dim_counter == 250){
      dim_counter = 0;
    }
    else {
      dim75();
      dim_counter += 1;
    }
  }
}
void dim25() {
  switch(dim_state2)
    {
    case 0: red_on = 1; break;
    case 1:
    case 2:
    case 3: red_on = 0; break;
    default: dim_state2 = 0; break;
    }
  led_changed = 1;
  led_update();
}
void dim50() {
  switch(dim_state2)
    {
    case 0:
    case 1: red_on = 1; break;
    case 2:
    case 3: red_on = 0; break;
    default: dim_state2 = 0;
    }
  led_changed = 1;
  led_update();
}
  
void dim75() {
  switch(dim_state2)
    {
    case 0: red_on = 0; break;
    case 1: 
    case 2: 
    case 3: red_on = 1; break;
    default: dim_state2 = 0;
    }
  led_changed = 1;
  led_update();
}
