#include "led_control.h"

static uint8_t led_mode;
static uint8_t last_led_mode;
static uint8_t stored_led_mode;
static uint8_t pos = 0;

static cRGB led_off = { .r = 0, .g = 0, .b = 0 };
static cRGB led_steady = { .r = 0, .g = 255, .b = 0};
static cRGB led_blue = { .r = 0, .g = 0, .b = 255 };
static cRGB led_dark_blue = { .r = 0, .g = 0, .b = 127 };
static cRGB led_bright_red = { .r = 255, .g = 0, .b = 0};
static cRGB led_breathe;
static cRGB rainbow;


static uint8_t rainbow_hue = 0;   //stores 0 to 614
static uint8_t rainbow_steps = 1; //number of hues we skip in a 360 range per update
static uint8_t rainbow_wave_steps =1; //number of hues we skip in a 360 range per update

static byte rainbow_saturation = 255;
static byte rainbow_value = 190;

static long rainbow_wave_ticks = 1; //delays between update
static long rainbow_ticks = 5; //delays between update
static long rainbow_current_ticks =0;
static uint8_t breathe_brightness = 0;    // how bright the LED is
static uint8_t breathe_fadeAmount = 1;    // how many pouint8_ts to fade the LED by

static uint8_t chase_pixels = 1;
static uint8_t chase_threshold = 6;
static uint8_t current_chase_counter = 0;
// End RGB stuff


void set_key_color(byte row, byte col, cRGB color) {
    implementation_led_set_crgb_at(row, col, color);
}

cRGB get_key_color(byte row, byte col) {
    return implementation_get_key_color(row, col);
}



void initialize_led_mode(uint8_t mode) {
    set_all_leds_to(led_off);
    if (mode == LED_MODE_OFF) {
        //        set_all_leds_to(led_off);
    } else if (mode == LED_MODE_HEATMAP) {
    } else if (mode == LED_MODE_BREATHE) {

    } else if (mode == LED_MODE_RAINBOW) {
    } else if (mode == LED_MODE_RAINBOW_WAVE) {
    } else if (mode == LED_MODE_CHASE) {
    } else if (mode == LED_MODE_STEADY) {
        set_all_leds_to(led_steady);
    }
}

void set_all_leds_to(cRGB color) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        implementation_led_set_crgb_at(i, color);
    }
}


void next_led_mode() {
    if (led_mode++ >= LED_MODES) {
        led_mode = 0;
    }
}

void set_led_mode(uint8_t mode) {
    led_mode = mode;
}




void update_leds(uint8_t numlock_enabled) {
    if (numlock_enabled) {
        if (led_mode != LED_SPECIAL_MODE_NUMLOCK) {
            stored_led_mode = led_mode;
        }
        led_mode = LED_SPECIAL_MODE_NUMLOCK;
    }
    if (!numlock_enabled &&
            led_mode == LED_SPECIAL_MODE_NUMLOCK
       ) {
        led_mode = stored_led_mode;
    }


    if (led_mode != last_led_mode) {
        initialize_led_mode(led_mode);
    }
    if (led_mode == LED_MODE_OFF) {
    } else if (led_mode == LED_MODE_HEATMAP) {
    } else if (led_mode == LED_MODE_BREATHE) {
        led_effect_breathe_update();
    } else if (led_mode == LED_MODE_RAINBOW) {
        led_effect_rainbow_update();
    } else if (led_mode == LED_MODE_RAINBOW_WAVE) {
        led_effect_rainbow_wave_update();
    } else if (led_mode == LED_MODE_CHASE) {
        led_effect_chase_update();
    } else if (led_mode == LED_MODE_STEADY) {
        led_effect_steady_update();
    } else if (led_mode == LED_SPECIAL_MODE_NUMLOCK) {
        led_effect_numlock_update();

    }

    last_led_mode = led_mode;
}



void led_effect_numlock_update() {
    for (uint8_t i = 0; i < 44; i++) {
        implementation_led_set_crgb_at(i, led_off);
    }
    for (uint8_t i = 44; i < LED_COUNT; i++) {
        implementation_led_set_crgb_at(i, led_bright_red);
    }
    led_compute_breath();
    implementation_led_set_crgb_at(60, led_breathe); // make numlock breathe
    implementation_led_sync();
}

void led_effect_steady_update() {
    implementation_led_sync();
}

void led_compute_breath() {
    // algorithm from http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
    breathe_brightness =  (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
    // change the brightness for next time through the loop:
    //breathe_brightness = breathe_brightness + breathe_fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (breathe_brightness == 0 || breathe_brightness == 150) {
        breathe_fadeAmount = -breathe_fadeAmount ;
    }


    SetHSV(led_breathe,200, 255, breathe_brightness);
}

void led_effect_breathe_update() {
    led_compute_breath();
    set_all_leds_to(led_breathe);
    implementation_led_sync();
}

void led_effect_chase_update() {
    if (current_chase_counter++ < chase_threshold) {
        return;
    }
    current_chase_counter = 0;
    implementation_led_set_crgb_at(pos - chase_pixels, led_off);
    implementation_led_set_crgb_at(pos, led_dark_blue);

    pos += chase_pixels;
    if (pos > LED_COUNT || pos < 0) {
        chase_pixels = -chase_pixels;
        pos += chase_pixels;
    }
    implementation_led_set_crgb_at(pos, led_blue);
    implementation_led_sync();
}

void led_effect_rainbow_update() {
    if (rainbow_current_ticks++ < rainbow_ticks) {
        return;
    } else {
        rainbow_current_ticks = 0;
    }
    SetHSV(rainbow,rainbow_hue, rainbow_saturation, rainbow_value);
    rainbow_hue += rainbow_steps;
    if (rainbow_hue >= 360)          {
        rainbow_hue %= 360;
    }
    set_all_leds_to(rainbow);
    implementation_led_sync();
}

void led_effect_rainbow_wave_update() {
    if (rainbow_current_ticks++ < rainbow_wave_ticks) {
        return;
    } else {
        rainbow_current_ticks = 0;
    }

    for (uint8_t i = 0; i < LED_COUNT; i++) {
        uint8_t key_hue = rainbow_hue +16*(i/4);
        if (key_hue >= 360)          {
            key_hue %= 360;
        }
        SetHSV(rainbow,key_hue, rainbow_saturation, rainbow_value);
        implementation_led_set_crgb_at(i,rainbow);
    }
    rainbow_hue += rainbow_wave_steps;
    if (rainbow_hue >= 360)          {
        rainbow_hue %= 360;
    }
    implementation_led_sync();
}

void led_bootup() {
    set_all_leds_to(led_off);

    led_type_letter(LED_K);
    led_type_letter(LED_E);
    led_type_letter(LED_Y);
    led_type_letter(LED_B);
    led_type_letter(LED_O);
    led_type_letter(LED_A);
    led_type_letter(LED_R);
    led_type_letter(LED_D);
    led_type_letter(LED_I);
    led_type_letter(LED_O);
    led_type_letter(LED_SPACE);
    led_type_letter(LED_0);
    led_type_letter(LED_PERIOD);
    led_type_letter(LED_9);
    led_mode = LED_MODE_RAINBOW_WAVE;


}

void led_type_letter(uint8_t letter) {
    implementation_led_set_crgb_at(letter,led_bright_red);
    implementation_led_sync();
    delay(250);
    implementation_led_set_crgb_at(letter,led_off);
    implementation_led_sync();
    delay(10);

}


void SetHSV(cRGB crgb, int hue, byte sat, byte val) {
    /* convert hue, saturation and brightness ( HSB/HSV ) to RGB
    The dim_curve is used only on brightness/value and on saturation (inverted).
    This looks the most natural.
    */

    int base;

    if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
        crgb.r = val;
        crgb.g = val;
        crgb.b = val;
    } else  {
        base = ((255 - sat) * val) >> 8;

        switch (hue / 60) {
        case 0:
            crgb.r = val;
            crgb.g = (((val - base)*hue) / 60) + base;
            crgb.b = base;
            break;

        case 1:
            crgb.r = (((val - base)*(60 - (hue % 60))) / 60) + base;
            crgb.g = val;
            crgb.b = base;
            break;

        case 2:
            crgb.r = base;
            crgb.g = val;
            crgb.b = (((val - base)*(hue % 60)) / 60) + base;
            break;

        case 3:
            crgb.r = base;
            crgb.g = (((val - base)*(60 - (hue % 60))) / 60) + base;
            crgb.b = val;
            break;

        case 4:
            crgb.r = (((val - base)*(hue % 60)) / 60) + base;
            crgb.g = base;
            crgb.b = val;
            break;

        case 5:
            crgb.r = val;
            crgb.g = base;
            crgb.b = (((val - base)*(60 - (hue % 60))) / 60) + base;
            break;
        }
    }
}
