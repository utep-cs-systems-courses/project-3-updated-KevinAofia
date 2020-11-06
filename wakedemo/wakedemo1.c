#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED_GREEN BIT6             // P1.6

short redrawScreen = 1;
u_int fontFgColor = COLOR_DARK_ORANGE;
u_int shapeColor = COLOR_BLUE;
u_int shape2Color = COLOR_VIOLET;
u_int shape3Color = COLOR_RED;
u_int shape4Color = COLOR_WHITE;


void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount == 250) {		//once/sec
    secCount = 0;
    //change square and text color
    fontFgColor = (fontFgColor == COLOR_DARK_ORANGE) ? COLOR_BLUE : COLOR_DARK_ORANGE;
    shapeColor = (shapeColor == COLOR_BLUE) ? COLOR_DARK_ORANGE : COLOR_BLUE;
    //erase black triangle
    for(u_char r = 0; r < 10; r++) {
      for(u_char c = 0; c <= r; c++) {
	drawPixel(screenWidth/2 + c, screenHeight/2 - r , COLOR_WHITE);
      }
    }
    redrawScreen = 1;
  }
  if (secCount == 150 || secCount == 200) {
    //change colors of violet triangle faster than other shapes
    shape2Color = (shape2Color == COLOR_VIOLET) ? COLOR_LIME_GREEN : COLOR_VIOLET;
    redrawScreen = 1;
  }
  if (secCount == 125) {
    shape3Color = (shape3Color == COLOR_RED) ? COLOR_WHITE : COLOR_RED;
    shape4Color = (shape4Color == COLOR_WHITE) ? COLOR_RED : COLOR_WHITE;
    redrawScreen = 1;
  }
  if (secCount == 50 || secCount == 100) {
    //draw black triangle
    for(u_char r = 0; r < 10; r++) {
      for(u_char c = 0; c <= r; c++) {
	drawPixel(screenWidth/2 + c, screenHeight/2 - r , COLOR_BLACK);
      }
    } 
    redrawScreen = 1;
  }
}
void main()
{
  P1DIR |= LED_GREEN;		/**< Green led on when CPU on */		
  P1OUT |= LED_GREEN;
  configureClocks();
  lcd_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_WHITE);
  while (1) {			//loop forever
    if (redrawScreen) {         
      redrawScreen = 0;         //reset redraw
      drawString5x7(20,20, "AOFIA RULES.", fontFgColor, COLOR_WHITE);
      //draw blue sqaure roughly in the middle
      for(u_char r = 0; r < 15; r++) {
	for(u_char c = 0; c < 15; c++) {
	  drawPixel(screenWidth/2 + c, screenHeight/2 + r , shapeColor);
	}
      }
      //draw violet triangle
      for(u_char r = 0; r < 10; r++) {
	for(u_char c = 0; c <= r; c++) {
	  drawPixel(screenWidth/2 - c, screenHeight/2 - r , shape2Color);
	}
      }
      //draw red triangle
      for(u_char r = 0; r < 10; r++) {
	for(u_char c = 0; c <= r; c++) {
	  drawPixel(screenWidth/2 - c, screenHeight/2 + r , shape3Color);
	}
      }
      //draw red square
      for(u_char r = 0; r < 10; r++) {
	for(u_char c = 0; c < 10; c++) {
	  drawPixel(screenWidth/2 - c, screenHeight/2 + r , shape4Color);
	}
      }
    }
    P1OUT &= ~LED_GREEN;	/* green off */
    or_sr(0x10);		/**< CPU OFF */
    P1OUT |= LED_GREEN;		/* green on */
  }
}
