#include "Model01.h"
    KeyboardioScanner Model01::leftHand(0);
    KeyboardioScanner Model01::rightHand(3);

Model01::Model01(void) {

}

void Model01::setup(void) {
    Wire.begin();
}


void Model01::led_set_crgb_at(uint8_t i, cRGB crgb) {
}

void Model01::led_sync() {
}


void Model01::scan_matrix() {
    uint8_t key_data;
    //scan the Keyboard matrix looking for connections
    for (byte row = 0; row < 4; row++) {

        for (byte col = 0; col < 4; col++) {
            handle_key_event(row, col, &key_data);
            handle_key_event(row, (8- 1) - col, &key_data);
        }
    }
}

