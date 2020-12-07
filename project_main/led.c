#include <msp430.h>
#include "led.h"

unsigned char red_on = 0, green_on = 0;
unsigned char led_changed = 0;

//                     [0]   [1]                  [0]   [1]
static char redVal[] = {0, LED_RED}, greenVal[] = {0, LED_GREEN};
//redVal[0] = 0, redVal[1] = LED_RED,greenVal[0] = 0, greenvalVal[1] = LED_GREEN

void led_init()
{
  P1DIR |= LEDS;      //turn on LED(s) transmisson(s) gate(s) to set as output 
  led_changed = 1;
  led_update();
}
void led_update()
{
  if (led_changed) {
    //bitwise op | will give us the value of redVal or'd with greenVal, EXAMPLE BELOW: 1ST CASE
    char ledFlags = redVal[red_on] | greenVal[green_on];

    //(      0xff    ^    LEDS              )| ledFlags
    //     1111 1111 ^ 0100 0001 = 1011 1110 | 0000 0000 = 1011 1110
    //     P1OUT                &= 1011 1110
    P1OUT &= (0xff^LEDS) | ledFlags; //you can see above how we cleared the led bits to off
    P1OUT |= ledFlags;		     //and here we turn the according bits back to true/on
    led_changed = 0;                 //we have changed the leds, so we can reset this val
  }
}

/*all functions below take advantage of the array redVal and greenVal, if we want to change
  specific LED, we set its array to one, which will in turn set the correct bit to true*/

void R_on(){
  red_on = 1;
  led_changed = 1;
  led_update();
}

void G_on(){
  green_on = 1;
  led_changed = 1;
  led_update();
}

void R_off(){
  red_on = 0;
  led_changed = 1;
  led_update();
}

void G_off(){
  green_on = 0;
  led_changed = 1;
  led_update();
}

void RG_on(){
  red_on = 1;
  green_on =1;
  led_changed = 1;
  led_update();
}

void RG_off(){
  red_on = 0;
  green_on = 0;
  led_changed = 1;
  led_update();
}
