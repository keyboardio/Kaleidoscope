#include "led_control.h"

uint8_t LEDControl::pos= 0;
uint8_t LEDControl::rainbow_hue=0;
uint8_t LEDControl::rainbow_steps = 1;
uint8_t LEDControl::rainbow_wave_steps=1;
long LEDControl::rainbow_current_ticks =0;
uint8_t LEDControl::breathe_brightness=0;
uint8_t LEDControl::breathe_fadeAmount=1;
uint8_t LEDControl::chase_pixels= 1;
uint8_t LEDControl::current_chase_counter = 0;

void LEDControl::set_key_color(byte row, byte col, cRGB color) {
    implementation_led_set_crgb_at(row, col, color);
}

cRGB LEDControl::get_key_color(byte row, byte col) {
    return implementation_get_key_color(row, col);
}



void LEDControl::initialize_led_mode(uint8_t mode) {
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

void LEDControl::set_all_leds_to(cRGB color) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        implementation_led_set_crgb_at(i, color);
    }
}


void LEDControl::next_mode() {
    if (led_mode++ >= LED_MODES) {
        led_mode = 0;
    }
}

void LEDControl::set_mode(uint8_t mode) {
    led_mode = mode;
}




void LEDControl::update(uint8_t current_keymap) {
    if (current_keymap == NUMPAD_KEYMAP) {
        if (led_mode != LED_SPECIAL_MODE_NUMLOCK) {
            stored_led_mode = led_mode;
        }
        led_mode = LED_SPECIAL_MODE_NUMLOCK;
    }
    if (current_keymap != NUMPAD_KEYMAP &&
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
        effect_breathe_update();
    } else if (led_mode == LED_MODE_RAINBOW) {
        effect_rainbow_update();
    } else if (led_mode == LED_MODE_RAINBOW_WAVE) {
        effect_rainbow_wave_update();
    } else if (led_mode == LED_MODE_CHASE) {
        effect_chase_update();
    } else if (led_mode == LED_MODE_STEADY) {
        effect_steady_update();
    } else if (led_mode == LED_SPECIAL_MODE_NUMLOCK) {
        effect_numlock_update();

    }

    last_led_mode = led_mode;
}



void LEDControl::effect_numlock_update() {
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

void LEDControl::effect_steady_update() {
    implementation_led_sync();
}

void LEDControl::led_compute_breath() {
    // algorithm from http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
    breathe_brightness =  (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
    // change the brightness for next time through the loop:
    //breathe_brightness = breathe_brightness + breathe_fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (breathe_brightness == 0 || breathe_brightness == 150) {
        breathe_fadeAmount = -breathe_fadeAmount ;
    }


    led_breathe.SetHSV(200, 255, breathe_brightness);
}

void LEDControl::effect_breathe_update() {
    led_compute_breath();
    set_all_leds_to(led_breathe);
    implementation_led_sync();
}

void LEDControl::effect_chase_update() {
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

void LEDControl::effect_rainbow_update() {
    if (rainbow_current_ticks++ < rainbow_ticks) {
        return;
    } else {
        rainbow_current_ticks = 0;
    }
    rainbow.SetHSV(rainbow_hue, rainbow_saturation, rainbow_value);
    rainbow_hue += rainbow_steps;
    if (rainbow_hue >= 360)          {
        rainbow_hue %= 360;
    }
    set_all_leds_to(rainbow);
    implementation_led_sync();
}

void LEDControl::effect_rainbow_wave_update() {
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
        rainbow.SetHSV(key_hue, rainbow_saturation, rainbow_value);
        implementation_led_set_crgb_at(i,rainbow);
    }
    rainbow_hue += rainbow_wave_steps;
    if (rainbow_hue >= 360)          {
        rainbow_hue %= 360;
    }
    implementation_led_sync();
}

void LEDControl::boot_animation() {
    set_all_leds_to(led_off);

    type_letter(LED_K);
    type_letter(LED_E);
    type_letter(LED_Y);
    type_letter(LED_B);
    type_letter(LED_O);
    type_letter(LED_A);
    type_letter(LED_R);
    type_letter(LED_D);
    type_letter(LED_I);
    type_letter(LED_O);
    type_letter(LED_SPACE);
    type_letter(LED_0);
    type_letter(LED_PERIOD);
    type_letter(LED_9);
    led_mode = LED_MODE_RAINBOW_WAVE;


}

void LEDControl::type_letter(uint8_t letter) {
    implementation_led_set_crgb_at(letter,led_bright_red);
    implementation_led_sync();
    delay(250);
    implementation_led_set_crgb_at(letter,led_off);
    implementation_led_sync();
    delay(10);

}

/* SetHSV from light_ws2812. Their source was:
 * getRGB() function based on <http://www.codeproject.com/miscctrl/CPicker.asp>
 * dim_curve idea by Jims
 * */
void LEDControl::SetHSV(cRGB *crgb, int hue, byte sat, byte val) {
    /* convert hue, saturation and brightness ( HSB/HSV ) to RGB
    */

    int base;

    if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
        crgb->r = val;
        crgb->g = val;
        crgb->b = val;
    } else  {
        base = ((255 - sat) * val) >> 8;

        switch (hue / 60) {
        case 0:
            crgb->r = val;
            crgb->g = (((val - base)*hue) / 60) + base;
            crgb->b = base;
            break;

        case 1:
            crgb->r = (((val - base)*(60 - (hue % 60))) / 60) + base;
            crgb->g = val;
            crgb->b = base;
            break;

        case 2:
            crgb->r = base;
            crgb->g = val;
            crgb->b = (((val - base)*(hue % 60)) / 60) + base;
            break;

        case 3:
            crgb->r = base;
            crgb->g = (((val - base)*(60 - (hue % 60))) / 60) + base;
            crgb->b = val;
            break;

        case 4:
            crgb->r = (((val - base)*(hue % 60)) / 60) + base;
            crgb->g = base;
            crgb->b = val;
            break;

        case 5:
            crgb->r = val;
            crgb->g = base;
            crgb->b = (((val - base)*(60 - (hue % 60))) / 60) + base;
            break;
        }
    }
}
