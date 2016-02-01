#include "KeyboardConfig.h"

Model01Beta_ KeyboardHardware;

// These global proxy functions are a cheap hack to avoid 
 void led_set_crgb_at(uint8_t i, cRGB crgb) {
    KeyboardHardware.led_set_crgb_at(i,crgb);
}
void led_sync(void) {
    KeyboardHardware.led_sync();
}
