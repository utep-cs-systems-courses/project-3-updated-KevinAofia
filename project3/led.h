#ifndef led_included
#define led_included
//on some boards the RED BIT0 and GREEN BIT6 are flipped

#define LED_RED BIT0        // P1.0 (0000 0001)
#define LED_GREEN BIT6      // P1.6 (0100 0000)
#define LEDS (BIT0 | BIT6)  //      (0100 0001)

extern unsigned char red_on, green_on;
extern unsigned char led_changed;

void led_init();
void led_update();

#endif // included
