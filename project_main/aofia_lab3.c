#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#include "p2switches.h"    

#include "aofia_lab3.h"

#define LED_GREEN BIT6             //P1.6
#define RED_LED BIT0               //P1.0

short redrawScreen = 1;
u_int fontFgColor = COLOR_RED; 
u_int color = COLOR_BLUE;
u_int color2 = COLOR_VIOLET;
u_int color3 = COLOR_RED;
u_int color4 = COLOR_BLACK;

u_int word_color = COLOR_LIME_GREEN;
/*
u_int word_color = COLOR_BLACK "SUPAFLY", fontFgColor, COLOR_CHOCOLATE);
u_int "MOTIVATED", fontFgColor, COLOR_CHOCOLATE);
u_int "BRANIAC", fontFgColor, COLOR_CHOCOLATE);
u_int "AMAZING", fontFgColor, COLOR_CHOCOLATE);
u_int "OUTSTANDING", fontFgColor, COLOR_CHOCOLATE);
u_int "FABULOUS", fontFgColor, COLOR_CHOCOLATE);
u_int "PERFECT", fontFgColor, COLOR_CHOCOLATE);
u_int "SPECIAL", fontFgColor, COLOR_CHOCOLATE);
u_int "CRAYCRAY", fontFgColor, COLOR_CHOCOLATE);
*/

char active_switches[5];    //defined in our header as extern for access in our main

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount == 250) {      //every 1 second 
    secCount = 0;             //reset counter every 250 interrupts per second
    fontFgColor = (fontFgColor == COLOR_RED) ? COLOR_BLUE : COLOR_RED; //switch2 colors
    redrawScreen = 1;
  }
  if (secCount == 150) {
    //change colors of violet triangle faster than other shapes
    color2 = (color2 == COLOR_VIOLET) ? COLOR_LIME_GREEN : COLOR_VIOLET;
    color3 = (color3 == COLOR_RED) ? COLOR_YELLOW : COLOR_RED;
    color4 = (color4 == COLOR_BLACK) ? COLOR_SIENNA : COLOR_BLACK;
    redrawScreen = 1;
  }
  if (secCount == 83) { //every 1/3 seconds, hide affirmations
    static char word_state = 0;
    switch(word_state) {
    case 0:
      word_color = (word_color == COLOR_LIME_GREEN) ? COLOR_BLACK : COLOR_LIME_GREEN;
      word_state = 1;
      break;
    case 1:
      word_color = (word_color == COLOR_BLACK) ? COLOR_HOT_PINK : COLOR_BLACK;
      word_state = 2;
      break;
    case 2:
      word_color = (word_color == COLOR_HOT_PINK) ? COLOR_BLACK : COLOR_HOT_PINK;
      word_state = 3;
      break;
    case 3:
      word_color = (word_color == COLOR_BLACK) ? COLOR_LIME_GREEN : COLOR_BLACK;
      word_state = 0;
      break;
    default:
      word_color = COLOR_LIME_GREEN;
      word_state = 0;
      break;
    }
    redrawScreen = 1;
  }
}
void main()
{
  P1DIR |= LED_GREEN;	      //Green led on when CPU on	
  P1OUT |= LED_GREEN;
  configureClocks();
  lcd_init();                 //init onboard LCD
  p2sw_init(15);              //init switches 1111
  enableWDTInterrupts();      //enable periodic interrupt 
  or_sr(0x8);	              //GIE(enable interrupts) where GIE is bit 3 on 16 bit register

  P1DIR |= RED_LED;
  clearScreen(COLOR_BLACK);       //Chocolate is good for the soul

  while (1) {			      //looping forever with occasional interrupt
    if (redrawScreen) {         
      redrawScreen = 0;               //redraw should be set again in our interrupt driven code
      update_switches();              //update our extern switches variable
      if((active_switches[0]) =='0') {
	drawString5x7(11,15, "SUPAFLY", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/3)-3,35, "MOTIVATED", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/2)+2,15, "BRANIAC", word_color, COLOR_BLACK);
	drawString5x7(11,55, "AMAZING", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/4),75, "OUTSTANDING", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/2)+2,55, "FABULOUS", word_color, COLOR_BLACK);
	drawString5x7(11,95, "PERFECT", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/3),115, "SPECIAL", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/2)+2,95, "CRAYCRAY", word_color, COLOR_BLACK);
      }
      if((active_switches[1]) =='1') { //this switch does not work, but will use it for inf case
	drawString5x7(14,(screenHeight)-10, "KEVIN SAMOA AOFIA", fontFgColor, COLOR_BLACK);
      }
      if((active_switches[2]) == '2') {
	draw_solid_diamond(20,20,COLOR_GOLDENROD);
      }
      if((active_switches[3]) == '3') {
	draw_multi_diamond((screenWidth/2),(screenHeight/2),color,color2,color3,color4);
      }
    }
    P1OUT &= ~LED_GREEN;	//green off
    or_sr(0x10);		//CPU OFF,0001 0000 bit 4 on the 16 bit register
    P1OUT |= LED_GREEN;		//green on
  }
}
void draw_solid_diamond(int col,int row,u_int color) {
  //draw upper right triangles
  for(u_char r = 0; r < 10; r++) {
    for(u_char c = 0; c <= r; c++) {
      drawPixel(col + c, row + r , color); //upper right
      drawPixel(col - c, row + r , color); //upper left
    }
  }
  //draw lower right triangles
  for(u_char r = 0; r <= 10; r++) {
    for(u_char c = 0; c <= 10 - r; c++) {
      //offset 10 rows
      drawPixel((col + c) , (row + r) + 10, color); //lower right
      drawPixel((col - c), (row + r) + 10, color); //lower left
    }
  }
}
void draw_multi_diamond(int col,int row, u_int color,u_int color2,u_int color3,u_int color4) {
  //draw upper right triangles
  for(u_char r = 0; r < 10; r++) {
    for(u_char c = 0; c <= r; c++) {
      drawPixel(col + c, row + r , color); //upper right
      drawPixel(col - c, row + r , color2); //upper left
    }
  }
  //draw lower right triangles
  for(u_char r = 0; r <= 10; r++) {
    for(u_char c = 0; c <= 10 - r; c++) {
      //offset 10 rows
      drawPixel((col + c) , (row + r) + 10, color3); //lower right
      drawPixel((col - c), (row + r) + 10, color4); //lower left
    }
  }
}
void update_switches() {              //code reused from original p2sw-demo
  u_int switches = p2sw_read(), i;
  for(i=0; i<4; i++) {
                                      //1 times 2 raised to i to activate correct port2 bit
    active_switches[i] = (switches & (1 << i)) ? '-' : '0' + i; //if activated, update switch val
  }
  active_switches[4] = 0;             //terminating char string
}
