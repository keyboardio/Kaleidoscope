#include <Arduino.h>
#include "WS2812.h"
#include "led_control.h"
#include "KeyboardConfig.h"

WS2812 LED(LED_COUNT); 
cRGB value;
byte intensity;
byte sign;
byte offset=0;


void setup_leds() {
	  LED.setOutput(LED_DATA_PIN);

	LED.setColorOrderRGB();  // Uncomment for RGB color order
	//LED.setColorOrderBRG();  // Uncomment for BRG color order
	//LED.setColorOrderGRB();  // Uncomment for GRB color order (Default; will be used if none other is defined.)

	intensity = 0;
	sign = 1;


}

  void update_leds_rgb(){

    if (1) {
  
  
  	byte i = 0;
	if (sign) {
		intensity+=1;
		if (intensity >= 128) 
			sign = 0; 
	}
	else {
		intensity-=1;
		if (intensity < 5){
			sign = 1;
            offset++;}
	}
if (offset>2) offset=0;

	while (i < LED_COUNT){


		if (((i+offset) % 3) == 0) {  // First LED, and every third after that
			value.b = 0; 
			value.g = 0; 
			value.r = intensity; // RGB Value -> Red Only
			LED.set_crgb_at(i, value); // Set value at LED found at index 0
		}
		else if (((i+offset )% 3)  == 1) { // Second LED, and every third after that
			value.b = 0; 
			value.g = intensity; 
			value.r = 0; // RGB Value -> Green Only
			LED.set_crgb_at(i, value); // Set value at LED found at index 0
		}
		else  { // Third LED, and every third after that
			value.b = intensity; 
			value.g = 0; 
			value.r = 0; // RGB Value -> Blue Only
			LED.set_crgb_at(i, value); // Set value at LED found at index 0
		}

        
		i++;
        }

        

	LED.sync(); // Sends the data to the LEDs
    }

	}


int red;
int green;
int blue;
int rednext=1;
int bluenext=1;
int greennext=1;
void update_leds_scroll() {
    int i=0;
    
    if(red==0&& blue==0&& green==0) {
        red=random(50);
        green=random(50);
        blue=random(50);
        rednext=bluenext=greennext=1;
    }

    if (red<0) {red=0;}
    if (green<0) {green=0;}
    if(blue<0){blue=0;}

    if (red+blue+green>100) {
        rednext=-1;
        bluenext=-1;
        greennext=-1;
    }
//    if (green>100) {
//        greennext=-1;
//    }
//    if (blue>100){
//        bluenext=-1;
//    }
    red+=rednext;
    green+=greennext;
    blue+=bluenext;

    value.r=red;
    value.g=green;
    value.b=blue;


	while (i < LED_COUNT){
			LED.set_crgb_at(i, value); // Set value at LED found at index 0
    i++;
    }
    LED.sync();
}
void update_leds() {
 update_leds_rgb();
}
