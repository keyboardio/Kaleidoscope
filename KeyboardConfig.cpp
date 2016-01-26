#include <Arduino.h>
#include "WS2812.h"
#include "KeyboardConfig.h"

WS2812 LED(LED_COUNT);

void implementation_setup_leds() {
    LED.setOutput(LED_DATA_PIN);
    LED.setColorOrderGRB();  // Uncomment for RGB color order
}

void implementation_led_set_crgb_at(byte row, byte col, cRGB color) {
    LED.set_crgb_at(key_led_map[row][col], color);
}

cRGB implementation_get_key_color(byte row, byte col) {
    return LED.get_crgb_at(key_led_map[row][col]);
}

void implementation_led_set_crgb_at(uint8_t i, cRGB crgb) {
    LED.set_crgb_at(i, crgb);
}

void implementation_led_sync() {
    LED.sync();
}
