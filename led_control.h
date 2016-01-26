
#pragma once
#include "WS2812.h"
#include "KeyboardConfig.h"

#define LED_MODES 7
#define LED_MODE_OFF 0
#define LED_MODE_STEADY 1
#define LED_MODE_BREATHE 2
#define LED_MODE_RAINBOW 3
#define LED_MODE_RAINBOW_WAVE 4
#define LED_MODE_HEATMAP 5
#define LED_MODE_CHASE 6

#define LED_SPECIAL_MODE_NUMLOCK 100

void setup_leds();
void update_leds(uint8_t numlock_enabled);
void set_all_leds_to(cRGB color);

void set_led_mode(uint8_t mode);
void next_led_mode();

void set_key_color(uint8_t row, uint8_t col, cRGB color);
cRGB get_key_color(uint8_t row, uint8_t col);

void led_compute_breath();
void led_effect_breathe_init();
void led_effect_rainbow_init();
void led_effect_chase_init();
void led_effect_steady_init();
void led_Effect_heatmap_init();

void led_effect_breathe_update();
void led_effect_rainbow_update();
void led_effect_rainbow_wave_update();
void led_effect_chase_update();
void led_effect_steady_update();
void led_effect_heatmap_update();
void led_effect_numlock_update();
void led_bootup();
void led_type_letter(uint8_t letter);

