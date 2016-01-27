#pragma once

#include <Arduino.h>
#include "cRGB.h"
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


class LEDControl {
  public:
    void next_led_mode();
    void led_bootup();
    void update_leds(uint8_t numlock_enabled);
    void led_type_letter(uint8_t letter);
    void set_led_mode(uint8_t mode);

  private:
     uint8_t led_mode;
     uint8_t last_led_mode;
     uint8_t stored_led_mode;
    static uint8_t pos;

     cRGB led_off = { .r = 0, .g = 0, .b = 0 };
     cRGB led_steady = { .r = 0, .g = 255, .b = 0};
     cRGB led_blue = { .r = 0, .g = 0, .b = 255 };
     cRGB led_dark_blue = { .r = 0, .g = 0, .b = 127 };
     cRGB led_bright_red = { .r = 255, .g = 0, .b = 0};
     cRGB led_breathe;
     cRGB rainbow;


    static uint8_t rainbow_hue;   //stores 0 to 614

    static uint8_t rainbow_steps; //number of hues we skip in a 360 range per update
    static uint8_t rainbow_wave_steps; //number of hues we skip in a 360 range per update

    static const byte rainbow_saturation = 255;
    static const byte rainbow_value = 190;

    static const long rainbow_wave_ticks = 1; //delays between update
    static const long rainbow_ticks = 5; //delays between update
    static long rainbow_current_ticks;

    static uint8_t breathe_brightness;    // how bright the LED is
    static uint8_t breathe_fadeAmount;    // how many points to fade the LED by
    static uint8_t chase_pixels;
    static uint8_t current_chase_counter;
    static const uint8_t chase_threshold = 6;
// End RGB stuff
    void set_key_color(uint8_t row, uint8_t col, cRGB color);
    cRGB get_key_color(uint8_t row, uint8_t col);
    void led_compute_breath();
    void led_effect_breathe_init();
    void led_effect_rainbow_init();
    void led_effect_chase_init();
    void led_effect_steady_init();
    void led_effect_heatmap_init();

    void led_effect_breathe_update();
    void led_effect_rainbow_update();
    void led_effect_rainbow_wave_update();
    void led_effect_chase_update();
    void led_effect_steady_update();
    void led_effect_heatmap_update();
    void led_effect_numlock_update();
    void set_all_leds_to(cRGB color);
    void SetHSV(cRGB crgb, int hue, byte sat, byte val);
    void initialize_led_mode(uint8_t mode);
};


