#ifndef LED_H__
#define LED_H__

//#define LED_BLUE 0
//#define LED_RED 1
//#define LED_YELLOW 2
//#define LED_GREEN 3
#define NUMBER_OF_LEDS 4

#define LED_RED 0
#define LED_YELLOW 1
#define LED_GREEN 2
#define LED_BLUE 3

#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3

#define BIT_BLUE (1 << 10)
#define BIT_RED (1 << 11)
#define BIT_YELLOW (1 << 28)
#define BIT_GREEN (1 << 29)

extern void led_init();
extern void led_on(int LED_ID);
extern void led_off(int LED_ID);
extern void led_toggle(int LED_ID);

#endif
