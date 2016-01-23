#include <Arduino.h>
#include "led_control.h"
cRGB value;
WS2812 LED(LED_COUNT);

#define USE_HSV

int led_mode;
int last_led_mode;
int stored_led_mode;
int pos = 0;
cRGB led_off;




cRGB led_steady;
cRGB led_blue;
cRGB led_dark_blue;
cRGB led_bright_red;

cRGB led_breathe;

// Begin RGB Stuff
cRGB rainbow;
int rainbow_hue = 0;   //stores 0 to 614
byte rainbow_steps = 1; //number of hues we skip in a 360 range per update
byte rainbow_wave_steps =1; //number of hues we skip in a 360 range per update

byte rainbow_saturation = 255;
byte rainbow_value = 190;

long rainbow_wave_ticks = 1; //delays between update
long rainbow_ticks = 5; //delays between update
long rainbow_current_ticks =0;
int breathe_brightness = 0;    // how bright the LED is
int breathe_fadeAmount = 1;    // how many points to fade the LED by

int chase_pixels = 1;
int chase_threshold = 6;
int current_chase_counter = 0;

// End RGB stuff

void setup_leds() {
    led_off.r = 0;
    led_off.g = 0;
    led_off.b = 0;
    led_steady.r = 0;
    led_steady.g = 255;
    led_steady.b = 0;
    led_blue.r = 0;
    led_blue.g = 0;
    led_blue.b = 255;

    led_dark_blue.r = 0;
    led_dark_blue.g = 0;
    led_dark_blue.b = 127;

    led_bright_red.r=255;
    led_bright_red.g=0;
    led_bright_red.b=0;




    LED.setOutput(LED_DATA_PIN);
    LED.setColorOrderGRB();  // Uncomment for RGB color order
}

void set_key_color(byte row, byte col, cRGB color) {
    LED.set_crgb_at(key_led_map[row][col], color);
}

cRGB get_key_color(byte row, byte col) {
    return LED.get_crgb_at(key_led_map[row][col]);
}



void initialize_led_mode(int mode) {
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
    for (int i = 0; i < LED_COUNT; i++) {
        LED.set_crgb_at(i, color);
    }

}


void next_led_mode() {
    led_mode++;
    if (led_mode >= LED_MODES) {
        led_mode = 0;
    }
}

void set_led_mode(int mode) {
    led_mode = mode;

}




void update_leds(int numlock_enabled) {
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

    for (int i = 0; i < 44; i++) {
        LED.set_crgb_at(i, led_off);
    }
    for (int i = 44; i < LED_COUNT; i++) {
        LED.set_crgb_at(i, led_bright_red);
    }
    led_compute_breath();
    LED.set_crgb_at(60, led_breathe); // make numlock breathe
    LED.sync();
}
void led_effect_steady_update() {
    LED.sync();
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


    led_breathe.SetHSV(200, 255, breathe_brightness);
}

void led_effect_breathe_update() {
    led_compute_breath();
    set_all_leds_to(led_breathe);
    LED.sync();
}

void
led_effect_chase_update() {
    if (current_chase_counter++ < chase_threshold) {
        return;
    }
    current_chase_counter = 0;
    LED.set_crgb_at(pos - chase_pixels, led_off);
    LED.set_crgb_at(pos, led_dark_blue);

    pos += chase_pixels;
    if (pos > LED_COUNT || pos < 0) {
        chase_pixels = -chase_pixels;
        pos += chase_pixels;

    }
    LED.set_crgb_at(pos, led_blue);
    LED.sync();
}







void led_effect_rainbow_update() {


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
    LED.sync();
}





void led_effect_rainbow_wave_update() {


    if (rainbow_current_ticks++ < rainbow_wave_ticks) {
        return;
    } else {
        rainbow_current_ticks = 0;
    }


    for (int i = 0; i < LED_COUNT; i++) {

        int key_hue = rainbow_hue +16*(i/4);
        if (key_hue >= 360)          {
            key_hue %= 360;
        }
        rainbow.SetHSV(key_hue, rainbow_saturation, rainbow_value);
        LED.set_crgb_at(i,rainbow);



    }
    rainbow_hue += rainbow_wave_steps;
    if (rainbow_hue >= 360)          {
        rainbow_hue %= 360;
    }
    LED.sync();
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

void led_type_letter(int letter) {
    LED.set_crgb_at(letter,led_bright_red);
    LED.sync();
    delay(400);
    LED.set_crgb_at(letter,led_off);
    LED.sync();
    delay(30);

}
