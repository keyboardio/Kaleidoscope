#include "KeyboardConfig.h"

HARDWARE_IMPLEMENTATION KeyboardHardware;

// These global proxy functions are a cheap hack to avoid
void led_set_crgb_at(uint8_t i, cRGB crgb) {
    KeyboardHardware.led_set_crgb_at(i,crgb);
}
void led_set_crgb_at(byte row, byte col, cRGB color) {
    KeyboardHardware.led_set_crgb_at(row,col,color);
}
cRGB led_get_crgb_at(uint8_t i) {
    return KeyboardHardware.led_get_crgb_at(i);
}
void led_sync(void) {
    KeyboardHardware.led_sync();
}
