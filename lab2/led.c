#include <msp430.h>
#include "led.h"

unsigned char red_on = 0, green_on = 0;
unsigned char led_changed = 0;
//#define BIT1 0x01;
//#define BIT6 0x40;

//                        (0000 0001)                (0100 0000)
static char redVal[] = {0, LED_RED}, greenVal[] = {0, LED_GREEN};


void led_init()
{
  P1DIR |= LEDS;   // bits attached to leds are output (0100 0001)
  led_changed = 1;
  led_update();
}

void led_update()
{
  if (led_changed) {
    char ledFlags = redVal[red_on] | greenVal[green_on];
    
    //((1111 1111)^(0100 0001))=(1011 1110) | with ledFlags which is updated by red_on/green_on
    P1OUT &= (0xff^LEDS) | ledFlags;    //clear bit for off leds, by forcing those to 0
    P1OUT |= ledFlags;		        // set bit for on leds, if any are true
    led_changed = 0;
  }
}
