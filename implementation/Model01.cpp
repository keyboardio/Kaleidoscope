#include <Arduino.h>
#include "WS2812.h"
#include "Model01.h"

Model01_::Model01_(void) {

}

void Model01_::setup(void) {
}


void Model01_::led_set_crgb_at(byte row, byte col, cRGB color) {
}

cRGB Model01_::get_key_color(byte row, byte col) {
}

void Model01_::led_set_crgb_at(uint8_t i, cRGB crgb) {
}

void Model01_::led_sync() {
}


void Model01_::scan_matrix() {
    //scan the Keyboard matrix looking for connections
    for (byte row = 0; row < LEFT_ROWS; row++) {

        for (byte col = 0; col < LEFT_COLS; col++) {
            handle_key_event(row, col);
            handle_key_event(row, (COLS - 1) - col);
        }
    }
}

