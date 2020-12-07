#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "p2switches.h"    
#include "aofia_lab3.h"
#include "shape.h"
#include "abCircle.h"
#include "buzzer.h"
#include "SM.h"
#include "led.h"

//defining/declaring our font, background, and diamond colors
u_int fontFgColor = COLOR_RED, color = COLOR_WHITE,color2 = COLOR_RED,color3 = COLOR_CYAN,
  color4 = COLOR_GOLD,word_color = COLOR_LIME_GREEN,bgColor = COLOR_BLACK;

char active_switches[5];                      //extern var defined in aofia_lab3.h

void siren();                                 //function .s using unsigned comparison
void siren2();                                //function .s using signed comparions

char blinkbuzz_state;                         //extern var defined in SM.h

Region fieldFence;		              //fence around playing field

short redrawScreen = 1;                       //redrawScreen set to initial start 

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
AbRectOutline fieldOutline = {	              //motion play field/range
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2 - 10, screenHeight/2 - 10}
};
Layer layer6 = {                              //layer with an colored shape
  (AbShape *)&circle10,
  {(screenWidth/2)+10, (screenHeight/2)+5},   //bit below & right of center
  {0,0}, {0,0},				      //last & next pos 
  COLOR_BLUE,                                 //color of shape
  0                                           //->next value
};
Layer layer5 = {	      
  (AbShape *)&circle8,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},
  COLOR_DARK_GREEN, 
  &layer6,
};
Layer layer4 = {		           
  (AbShape *)&circle6,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},
  COLOR_PURPLE, 
  &layer5,
};
Layer layer3 = { 
  (AbShape *)&circle4,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},				   
  COLOR_WHITE, 
  &layer4,
};
Layer fieldLayer = {		            //cast a boundary layer
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},          //center
  {0,0}, {0,0},				    //last & next pos
  COLOR_BLACK,                              //boundary matches bg appear "invisible"
  &layer3,
};
Layer layer1 = {		           
  (AbShape *)&circle2,
  {screenWidth/2, screenHeight/2},         
  {0,0}, {0,0},
  COLOR_RED,
  &fieldLayer,
};
Layer layer0 = {		           
  (AbShape *)&circle12,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},
  COLOR_LIME_GREEN,
  &layer1,
};
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//Moving Layer,L.L. layer ref.,Veloc. rep. one iter. of change(dir. & mag.)
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
                                         //initial value of {0,0} will be overwritten
MovLayer ml6 = { &layer6, {1,2}, 0 };    //added circles to linked list
MovLayer ml5 = { &layer5, {2,1}, &ml6 };
MovLayer ml4 = { &layer4, {1,1}, &ml5 };
MovLayer ml3 = { &layer3, {2,2}, &ml4 };        
MovLayer ml1 = { &layer1, {1,3}, &ml3 }; 
MovLayer ml0 = { &layer0, {3,1}, &ml1 };
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void movLayerDraw(MovLayer *movLayers, Layer *layers) {
  int row, col;
  MovLayer *movLayer;
  and_sr(~8);	 //disable interrupts (GIE OFF)
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { //for each moving layer
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);	 //disable interrupts (GIE ON)
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { //for each moving layer
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1],
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer;
	     probeLayer = probeLayer->next) { //probe all layers, in order
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break;
	  }                                  //if probe check
	}                                    //for checking all layers at col, row
	lcd_writeColor(color);
      }                                      //for col
    }                                        //for row
  }                                          //for moving layer being updated
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void mlAdvance(MovLayer *ml, Region *fence) {
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (2*velocity);
      }                                 //if outside of fence
    }                                   //for axis
    ml->layer->posNext = newPos;
  }                                     //for ml
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void draw_stick_figure() {
  draw_solid_diamond(15,80,COLOR_RED);                    //left hand
  draw_solid_diamond(105,80,COLOR_RED);                   //right hand
  draw_multi_diamond(60,75,color,color2,color3,color4);   //body
  for(u_char r = 0; r < 20; r++) {
    for(u_char c = 0; c <= r; c++) {
      drawPixel(80 + c, 120 + r , COLOR_RED);             //right foot
      drawPixel(40 - c, 120 + r , COLOR_RED);             //left foot
    }
  }
  for(u_char r = 0; r <= 25; r++) {
    for(u_char c = 0; c <= 2; c++) {
      drawPixel(80+c,95+r, COLOR_BLUE);                   //right leg
      drawPixel(40-c,95+r, COLOR_BLUE);                   //left leg
      drawPixel(59+c,50+r, COLOR_BLUE);                   //upper spine
      drawPixel(16-c,56+r, COLOR_BLUE);                   //left arm
      drawPixel(104+c,56+r, COLOR_BLUE);                  //right arm
    }
  }
  for(u_char r = 0; r <= 5; r++) {
    for(u_char c = 0; c <= 2; c++) {
      drawPixel(60+c,95+r, COLOR_BLUE);                  //lower spine
    }
  }
  for(u_char r = 0; r <= 2; r++) {
    for(u_char c = 0; c <= 40; c++) {
      drawPixel(39+c,100+r, COLOR_BLUE);                 //leg connector
    }
  }
  for(u_char r = 0; r <= 2; r++) {
    for(u_char c = 0; c <= 88; c++) {
      drawPixel(16+c,56+r, COLOR_BLUE);                  //arm connector
    }
  }
  for(u_char r = 0; r <= 25; r++) {
    for(u_char c = 0; c <= 25; c++) {
      drawPixel(47+c,25+r, COLOR_RED);                   //head
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void update_switches() {              //code reused from original p2sw-demo
  u_int switches = p2sw_read(), i;
  for(i=0; i<4; i++) {
                                     //1 times 2 raised to i to activate correct port2 bit
    active_switches[i] = (switches & (1 << i)) ? '-' : '0' + i; //if activated, update switch val
  }
  active_switches[4] = 0;             //terminating char string
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void wdt_c_handler()
{
  static int affirmationsCount = 0;   //interrupts for sw1 affirmation colors
  static int blinkbuzzCount = 0;      //interuppts for sw1 blinkbuzz state
  static int circleCount = 0;         //interrupts for sw3 moving of layers
  static int dimCount = 0;            //interrupts for sw3 dim state
  static int diamondCount = 0;        //interrupts for sw4 diamond color
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  affirmationsCount++;
  blinkbuzzCount++;
  circleCount++;
  diamondCount++;
  dimCount++;
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  if (diamondCount == 250) {           //interrupt every second 
    diamondCount = 0;             
    fontFgColor = (fontFgColor == COLOR_RED) ? COLOR_BLUE : COLOR_RED; 
    color = (color == COLOR_WHITE) ? COLOR_LIME_GREEN : COLOR_WHITE;
    redrawScreen = 1;
  }
  if (diamondCount == 125) {           //interrupt every 1/2 second
    color2 = (color2 == COLOR_RED) ? COLOR_SKY_BLUE : COLOR_RED;
    color3 = (color3 == COLOR_CYAN) ? COLOR_DARK_VIOLE : COLOR_CYAN;
    redrawScreen = 1;
  }
  if (diamondCount == 84) {           //interrupt every 1/3 second
    color4 = (color4 == COLOR_GOLD) ? COLOR_TAN : COLOR_GOLD;
    redrawScreen = 1;
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  if (affirmationsCount == 100) {    //every 100/250 interrupts, hide affirmations
    static char word_state = 0;
    switch(word_state) {
    case 0:
      word_color = (word_color == COLOR_LIME_GREEN) ? COLOR_BLACK : COLOR_LIME_GREEN;
      word_state = 1;
      affirmationsCount = 0;
      break;
    case 1:
      word_color = (word_color == COLOR_BLACK) ? COLOR_HOT_PINK : COLOR_BLACK;
      word_state = 2;
      affirmationsCount = 0;
      break;
    case 2:
      word_color = (word_color == COLOR_HOT_PINK) ? COLOR_BLACK : COLOR_HOT_PINK;
      word_state = 3;
      affirmationsCount = 0;
      break;
    case 3:
      word_color = (word_color == COLOR_BLACK) ? COLOR_LIME_GREEN : COLOR_BLACK;
      word_state = 0;
      affirmationsCount = 0;
      break;
    default:
      word_color = COLOR_LIME_GREEN;
      word_state = 0;
      break;
    }
    redrawScreen = 1;
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  if(blinkbuzzCount == 83) {         //every 1/3 second
    blinkbuzz_state++;
    blinkbuzzCount = 0;
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  if(circleCount == 25) {           //every 1/10 second
    mlAdvance(&ml0, &fieldFence);
    if(p2sw_read()){
      redrawScreen = 1;
    }
    circleCount = 0;
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  if(dimCount == 125){              //every 1/2 second
    dim_state++;
    dimCount = 0;
  }
}
void main()
{
  led_init();                 //init our leds but do not turn any on
  configureClocks();
  lcd_init();                 //init onboard LCD
  p2sw_init(15);              //init switches 1111
  buzzer_init();              //init buzzer
  layerInit(&layer0);         //init layers
  layerDraw(&layer0);         //init layers
  layerGetBounds(&fieldLayer, &fieldFence);  //init layers
  enableWDTInterrupts();      //enable periodic interrupt 
  or_sr(0x8);	              //GIE(enable interrupts) where GIE is bit 3 on 16 bit register
 
  clearScreen(COLOR_BLACK);   //main black screen
  while (1) {		      //looping forever with occasional interrupt
    if (redrawScreen) {         
      redrawScreen = 0;       //redraw should be set again in our interrupt driven code
      update_switches();      //update our extern active_switches variable
      //////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////
      if((active_switches[0]) =='0') {       //draw affirmations and set buzzer
	drawString5x7(11,15, "SUPAFLY", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/3)-3,35, "MOTIVATED", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/2)+2,15, "BRANIAC", word_color, COLOR_BLACK);
	drawString5x7(11,55, "AMAZING", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/4),75, "OUTSTANDING", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/2)+2,55, "FABULOUS", word_color, COLOR_BLACK);
	drawString5x7(11,95, "PERFECT", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/3),115, "SPECIAL", word_color, COLOR_BLACK);
	drawString5x7((screenWidth/2)+2,95, "AWESOME", word_color, COLOR_BLACK);
	blinkbuzz(); //will change states as fast as our while loop and occasional interrupt
      }
      //////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////
      if((active_switches[1]) =='1') {       //draw footer and change colors
	drawString8x12(13,(screenHeight-12), "KEVIN AOFIA", fontFgColor, COLOR_BLACK);
	state_advance();                     //dim current state until interrupt advances state
	redrawScreen = 1;
      }
      //////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////
      if((active_switches[2]) == '2') {     //draw shapes in motion
	siren2();                           //siren with signed comparison
	movLayerDraw(&ml0, &layer0);        
      }
      //////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////
      if((active_switches[3]) == '3') {     //draw stick man and activate siren
	siren();                            //siren with unsigned comparison
	draw_stick_figure();
      }
      else {
	//if switches 1,3,4 are up, turn off buzzer,turn off RED_LED
	if((active_switches[0] != '0')&&(active_switches[2] != '2')&&(active_switches[3] != '3')){
	  buzzer_set_period(0);
	  R_off();
	}
      }
    }                //CPU off, we will compare with &redrawScreen upon waking up
    or_sr(0x10);     //CPU OFF,0001 0000 bit 4 on the 16 bit sr_register
  }
}
