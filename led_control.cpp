#include <Arduino.h>
#include "led_control.h"
cRGB value;
WS2812 LED(LED_COUNT);

int led_mode;
int last_led_mode;

cRGB led_off;


cRGB led_steady;



cRGB led_breathe;


void setup_leds() {
led_off.r=0;
led_off.g=0;
led_off.b=0;
led_steady.r=30;
led_steady.g=30;
led_steady.b=30;
    LED.setOutput(LED_DATA_PIN);
	LED.setColorOrderRGB();  // Uncomment for RGB color order
    led_mode=LED_MODE_STEADY;
}

byte key_to_led(byte row, byte col) {
    return key_led_map[row][col];
}

void set_key_color(byte row, byte col, cRGB color){
    LED.set_crgb_at(key_to_led(row,col),color);
}

cRGB get_key_color(byte row, byte col){
    return LED.get_crgb_at(key_to_led(row,col));
}


  void update_leds_rgb(){
			//LED.set_crgb_at(i, value); // Set value at LED found at index 0
	LED.sync(); // Sends the data to the LEDs

	}

void initialize_led_mode(int mode) {
    if (mode == LED_MODE_OFF) {
        set_all_leds_to(led_off);
    } else if (mode == LED_MODE_HEATMAP) {
    } else if (mode == LED_MODE_BREATHE) {

    } else if (mode == LED_MODE_RAINBOW) {
    } else if (mode == LED_MODE_CHASE) {
    } else if (mode == LED_MODE_STEADY) {
        set_all_leds_to(led_steady);
    }
}

void set_all_leds_to(cRGB color) {
        for(int i=0; i<LED_COUNT;i++) {
            LED.set_crgb_at(i,color);
        }

}
void update_leds() {
    if (led_mode != last_led_mode) {
        initialize_led_mode(led_mode);
    }
    if (led_mode == LED_MODE_OFF) {
    } else if (led_mode == LED_MODE_HEATMAP) {
    } else if (led_mode == LED_MODE_BREATHE) {
        // algorithm from http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
        int brightness = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
        led_breathe.r=brightness;
        led_breathe.g=brightness;
        led_breathe.b=brightness;

    } else if (led_mode == LED_MODE_RAINBOW) {
    } else if (led_mode == LED_MODE_CHASE) {
    } else if (led_mode == LED_MODE_STEADY) {
    }

        LED.sync();
        last_led_mode = led_mode;
}
