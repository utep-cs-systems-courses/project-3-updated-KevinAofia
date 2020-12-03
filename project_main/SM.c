#include <msp430.h>
#include "SM.h"
#include "led.h"
#include "buzzer.h"

char blinkbuzz_state = 0;  //extern

char dim_state = 0;        //extern
char start = 0;            //extern

void blinkbuzz() {
  switch(blinkbuzz_state)
    {
    case 0:
      R_on();
      buzzer_set_period(4500);
      //blinkbuzz_state = 1;
      break;
    case 1:
      R_off();
      buzzer_set_period(2000);
      //blinkbuzz_state = 0;
      break;
    default:
      blinkbuzz_state = 0;
    }
}

void state_advance(){
  switch(dim_state)
    {
    case 0:off();start=0;break;
    case 1:d10();start++;break;
    case 2:d20();start++;break;
    case 3:d30();start++;break;
    case 4:d40();start++;break;
    case 5:d50();start++;break;
    case 6:d60();start++;break;
    case 7:d70();start++;break;
    case 8:d80();start++;break;
    case 9:d90();start++;break;
    case 10:on();start++;break;
    default:dim_state = 0;
    }
}
void off(){
  G_off();
}
void d10() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_off();break;
    case 2:G_off();break;
    case 3:G_off();break;
    case 4:G_off();break;
    case 5:G_off();break;
    case 6:G_off();break;
    case 7:G_off();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d20() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_off();break;
    case 3:G_off();break;
    case 4:G_off();break;
    case 5:G_off();break;
    case 6:G_off();break;
    case 7:G_off();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d30() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_on();break;
    case 3:G_off();break;
    case 4:G_off();break;
    case 5:G_off();break;
    case 6:G_off();break;
    case 7:G_off();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d40() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_on();break;
    case 3:G_on();break;
    case 4:G_off();break;
    case 5:G_off();break;
    case 6:G_off();break;
    case 7:G_off();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d50() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_on();break;
    case 3:G_on();break;
    case 4:G_on();break;
    case 5:G_off();break;
    case 6:G_off();break;
    case 7:G_off();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d60() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_on();break;
    case 3:G_on();break;
    case 4:G_on();break;
    case 5:G_on();break;
    case 6:G_off();break;
    case 7:G_off();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d70() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_on();break;
    case 3:G_on();break;
    case 4:G_on();break;
    case 5:G_on();break;
    case 6:G_on();break;
    case 7:G_off();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d80() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_on();break;
    case 3:G_on();break;
    case 4:G_on();break;
    case 5:G_on();break;
    case 6:G_on();break;
    case 7:G_on();break;
    case 8:G_off();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void d90() {
  switch(start)
    {
    case 0:G_on();break;
    case 1:G_on();break;
    case 2:G_on();break;
    case 3:G_on();break;
    case 4:G_on();break;
    case 5:G_on();break;
    case 6:G_on();break;
    case 7:G_on();break;
    case 8:G_on();break;
    case 9:G_off();break;
    default:start=0;break;
    }
}
void on(){
  G_on();
}
