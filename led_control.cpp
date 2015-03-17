#include <Arduino.h>
#include "led_control.h"
cRGB value;
WS2812 LED(LED_COUNT);

int led_mode;
int last_led_mode;
int pos=0;
cRGB led_off;


cRGB led_steady;
cRGB led_blue;
cRGB led_dark_blue;


cRGB led_breathe;


void setup_leds() {
led_off.r=0;
led_off.g=0;
led_off.b=0;
led_steady.r=160;
led_steady.g=160;
led_steady.b=160;
led_blue.r=0;
led_blue.g=0;
led_blue.b=255;

led_dark_blue.r=0;
led_dark_blue.g=0;
led_dark_blue.b=127;


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

int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by
int chase_pixels = 1;
int chase_threshold = 6;
int current_chase_counter=0;
void update_leds() {
    if (led_mode != last_led_mode) {
        initialize_led_mode(led_mode);
    }
    if (led_mode == LED_MODE_OFF) {
    } else if (led_mode == LED_MODE_HEATMAP) {
    } else if (led_mode == LED_MODE_BREATHE) {
        // algorithm from http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
      //  int brightness =  (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
         // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 160) {
    fadeAmount = -fadeAmount ; 
  }     

       
       
        led_breathe.r=brightness;
        led_breathe.g=brightness;
        led_breathe.b=brightness;
        set_all_leds_to(led_breathe);
        LED.sync();

    } else if (led_mode == LED_MODE_RAINBOW) {
    } else if (led_mode == LED_MODE_CHASE) {
      if(current_chase_counter++<chase_threshold) {
        return;
      }
      current_chase_counter=0;
      LED.set_crgb_at(pos-chase_pixels,led_off);
      LED.set_crgb_at(pos,led_dark_blue);

      pos+=chase_pixels;
      if (pos>LED_COUNT||pos<0) {
         chase_pixels = -chase_pixels; 
    pos+=chase_pixels;
    
    }
      LED.set_crgb_at(pos,led_blue);
        LED.sync();
      
    } else if (led_mode == LED_MODE_STEADY) {
        LED.sync();
    }

        last_led_mode = led_mode;
}
