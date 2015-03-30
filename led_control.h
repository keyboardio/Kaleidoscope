#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "WS2812.h"
#define LED_MODES 6
#define LED_MODE_OFF 0
#define LED_MODE_STEADY 1
#define LED_MODE_BREATHE 2
#define LED_MODE_RAINBOW 3
#define LED_MODE_HEATMAP 4
#define LED_MODE_CHASE 5

#define LED_DATA_PIN  4

#define LED_COUNT 64

static const int key_led_map[4][16] = 
{
    {3,4,11,12,19,20,27,28,     35,39,40,47,48,55,56,63},
    {2,5,10,13,18,21,26,29,     34,38,41,46,49,54,57,62},
    {1,6,9,14, 17,22,25,30,     33,37,42,45,50,53,58,61},
    {0,7,8,15,16,23,24,31,      32,36,43,44,51,52,59,60},
};



void setup_leds();
void update_leds();
void set_all_leds_to(cRGB color);

void set_led_mode(int mode);
void next_led_mode();

byte key_to_led(byte row, byte col);
void set_key_color(byte row, byte col);
cRGB get_key_color(byte row, byte col);

void led_effect_breathe_init();
void led_effect_rainbow_init();
void led_effect_chase_init();
void led_effect_steady_init();
void led_Effect_heatmap_init();

void led_effect_breathe_update();
void led_effect_rainbow_update();
void led_effect_chase_update();
void led_effect_steady_update();
void led_Effect_heatmap_update();

#endif
