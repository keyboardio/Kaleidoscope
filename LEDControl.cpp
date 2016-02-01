#include "LEDControl.h"






LEDControl_::LEDControl_(void) {

}


void LEDControl_::set_key_color(byte row, byte col, cRGB color) {
    KeyboardHardware.led_set_crgb_at(row, col, color);
}

cRGB LEDControl_::get_key_color(byte row, byte col) {
    return KeyboardHardware.get_key_color(row, col);
}

void LEDControl_::initialize_led_mode(uint8_t mode) {
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

void LEDControl_::set_all_leds_to(cRGB color) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        KeyboardHardware.led_set_crgb_at(i, color);
    }
}


void LEDControl_::next_mode() {
    if (led_mode++ >= LED_MODES) {
        led_mode = 0;
    }
}

void LEDControl_::set_mode(uint8_t mode) {
    led_mode = mode;
}




void LEDControl_::update(uint8_t current_keymap) {
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



void LEDControl_::effect_numlock_update() {
    for (uint8_t i = 0; i < 44; i++) {
        KeyboardHardware.led_set_crgb_at(i, led_off);
    }
    for (uint8_t i = 44; i < LED_COUNT; i++) {
        KeyboardHardware.led_set_crgb_at(i, led_bright_red);
    }
    led_compute_breath();
    KeyboardHardware.led_set_crgb_at(60, led_breathe); // make numlock breathe
    KeyboardHardware.led_sync();
}

void LEDControl_::effect_steady_update() {
    KeyboardHardware.led_sync();
}

void LEDControl_::led_compute_breath() {
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

void LEDControl_::effect_breathe_update() {
    led_compute_breath();
    set_all_leds_to(led_breathe);
    KeyboardHardware.led_sync();
}

void LEDControl_::effect_chase_update() {
    if (current_chase_counter++ < chase_threshold) {
        return;
    }
    current_chase_counter = 0;
    KeyboardHardware.led_set_crgb_at(pos - chase_pixels, led_off);
    KeyboardHardware.led_set_crgb_at(pos, led_dark_blue);

    pos += chase_pixels;
    if (pos > LED_COUNT || pos < 0) {
        chase_pixels = -chase_pixels;
        pos += chase_pixels;
    }
    KeyboardHardware.led_set_crgb_at(pos, led_blue);
    KeyboardHardware.led_sync();
}

void LEDControl_::effect_rainbow_update() {
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
    KeyboardHardware.led_sync();
}

void LEDControl_::effect_rainbow_wave_update() {
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
        KeyboardHardware.led_set_crgb_at(i,rainbow);
    }
    rainbow_hue += rainbow_wave_steps;
    if (rainbow_hue >= 360)          {
        rainbow_hue %= 360;
    }
    KeyboardHardware.led_sync();
}

void LEDControl_::boot_animation() {
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

void LEDControl_::type_letter(uint8_t letter) {
    KeyboardHardware.led_set_crgb_at(letter,led_bright_red);
    KeyboardHardware.led_sync();
    delay(250);
    KeyboardHardware.led_set_crgb_at(letter,led_off);
    KeyboardHardware.led_sync();
    delay(10);

}


LEDControl_ LEDControl;
