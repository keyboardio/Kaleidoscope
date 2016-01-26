
#pragma once


#include "WS2812.h"
#define LED_MODES 7
#define LED_MODE_OFF 0
#define LED_MODE_STEADY 1
#define LED_MODE_BREATHE 2
#define LED_MODE_RAINBOW 3
#define LED_MODE_RAINBOW_WAVE 4
#define LED_MODE_HEATMAP 5
#define LED_MODE_CHASE 6

#define LED_SPECIAL_MODE_NUMLOCK 100
#define LED_DATA_PIN  4

#define LED_COUNT 64

static const uint8_t key_led_map[4][16] = {
    {3,4,11,12,19,20,26,27,     36,37,43,44,51,52,59,60},
    {2,5,10,13,18,21,31,28,     35,32,42,45,50,53,58,61},
    {1,6,9,14, 17,22,25,29,     34,38,41,46,49,54,57,62},
    {0,7,8,15,16,23,24,30,      33,39,40,47,48,55,56,63},
};



#define LED_PGDN 0
#define LED_PGUP 1
#define LED_BACKTICK 2
#define LED_LED 3
#define LED_1 4
#define LED_Q 5
#define LED_A 6
#define LED_Z 7
#define LED_X 8
#define LED_S 9
#define LED_W 10
#define LED_2 11
#define LED_3 12
#define LED_E 13
#define LED_D 14
#define LED_C 15
#define LED_V 16
#define LED_F 17
#define LED_R 18
#define LED_4 19
#define LED_5 20
#define LED_T 21
#define LED_REC_MACRO 22
#define LED_B 23
#define LED_ESC 24
#define LED_TAB 25
#define LED_REC 26
#define LED_L_FN 27
#define LED_L_CTRL 28
#define LED_DEL 29
#define LED_CMD 30
#define LED_L_SHIFT 31
#define LED_R_SHIFT 32
#define LED_ALT 33
#define LED_SPACE 34
#define LED_CTRL 35
#define LED_R_FN 36
#define LED_ANY 37
#define LED_RETURN 38
#define LED_BUTTERFLY 39
#define LED_N 40
#define LED_H 41
#define LED_Y 42
#define LED_6 43
#define LED_7 44
#define LED_U 45
#define LED_J 46
#define LED_M 47
#define LED_COMMA 48
#define LED_K 49
#define LED_I 50
#define LED_8 51
#define LED_9 52
#define LED_O 53
#define LED_L 54
#define LED_PERIOD 55
#define LED_SLASH 56
#define LED_SEMICOLON 57
#define LED_P 58
#define LED_0 59
#define LED_NUM 60
#define LED_EQUALS 61
#define LED_APOSTROPHE 62
#define LED_MINUS 63

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

