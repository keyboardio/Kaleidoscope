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


class LEDControl_ {
  public:
    LEDControl_(void);
    void next_mode();
    void boot_animation();
    void update(uint8_t current_keymap);
    void type_letter(uint8_t letter);
    void set_mode(uint8_t mode);

  private:
    uint8_t led_mode = 0;
    uint8_t last_led_mode = 0;
    uint8_t stored_led_mode = 0;
    uint8_t pos = 0;




    cRGB led_off = { .r = 0, .g = 0, .b = 0 };
    cRGB led_steady = { .r = 0, .g = 255, .b = 0};
    cRGB led_blue = { .r = 0, .g = 0, .b = 255 };
    cRGB led_dark_blue = { .r = 0, .g = 0, .b = 127 };
    cRGB led_bright_red = { .r = 255, .g = 0, .b = 0};
    cRGB led_breathe;
    cRGB rainbow;


    uint16_t rainbow_hue = 0;   //stores 0 to 614

    static const uint8_t rainbow_steps = 1; //number of hues we skip in a 360 range per update
    static const uint8_t rainbow_wave_steps=1; //number of hues we skip in a 360 range per update

    static const byte rainbow_saturation = 255;
    static const byte rainbow_value = 255;

    static const long rainbow_wave_ticks = 0; //delays between update
    static const long rainbow_ticks = 5; //delays between update
    long rainbow_current_ticks=0;

    uint8_t breathe_brightness = 0;    // how bright the LED is
    int8_t breathe_fadeAmount=1;    // how many points to fade the LED by (can be negative)
    int8_t chase_pixels =1; //negative values when it's going backwar
    uint8_t current_chase_counter = 0;
    static const uint8_t chase_threshold = 6;
// End RGB stuff
    void led_compute_breath();
    void effect_breathe_init();
    void effect_rainbow_init();
    void effect_chase_init();
    void effect_steady_init();
    void effect_heatmap_init();

    void effect_breathe_update();
    void effect_rainbow_update();
    void effect_rainbow_wave_update();
    void effect_chase_update();
    void effect_steady_update();
    void effect_heatmap_update();
    void effect_numlock_update();
    void set_all_leds_to(cRGB color);
    void initialize_led_mode(uint8_t mode);
};

extern LEDControl_ LEDControl;
