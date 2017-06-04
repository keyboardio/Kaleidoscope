#pragma once

// Keyboard Leds
enum KeyboardLeds : uint8_t {
  LED_NUM_LOCK            = (1 << 0),
  LED_CAPS_LOCK           = (1 << 1),
  LED_SCROLL_LOCK         = (1 << 2),
  LED_COMPOSE                     = (1 << 3),
  LED_KANA                        = (1 << 4),
  LED_POWER                       = (1 << 5),
  LED_SHIFT                       = (1 << 6),
  LED_DO_NOT_DISTURB      = (1 << 7),
};

