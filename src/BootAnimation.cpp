#include "BootAnimation.h"
#include "Kaleidoscope-LEDControl.h"
#include "EEPROM.h"

#define EEPROM_BOOT_ANIMATION_LOCATION 1

#ifdef ARDUINO_AVR_MODEL01
static void
type_letter(uint8_t letter) {
  LEDControl.led_set_crgb_at(letter, {255, 0, 0});
  LEDControl.led_sync();
  delay(250);
  LEDControl.led_set_crgb_at(letter, {0, 0, 0});
  LEDControl.led_sync();
  delay(10);
}
#endif

void
bootAnimation(void) {
#ifdef ARDUINO_AVR_MODEL01
  if (EEPROM.read(EEPROM_BOOT_ANIMATION_LOCATION))
    return;

  LEDControl.set_all_leds_to(0, 0, 0);
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

  EEPROM.update(EEPROM_BOOT_ANIMATION_LOCATION, 1);
#endif
}
