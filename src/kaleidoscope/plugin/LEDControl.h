/* Kaleidoscope-LEDControl - LED control plugin for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Kaleidoscope.h>

#define LED_TOGGLE   B00000001  // Synthetic, internal

#define Key_LEDEffectNext Key(0, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE)
#define Key_LEDEffectPrevious Key(1, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE)

#define _DEPRECATED_MESSAGE_LED_CONTROL_MODE_ADD                               \
  "LEDControl::mode_add(LEDMode *mode) is deprecated. LEDModes are now \n"     \
  "automatically registered. You can safely remove any calls to \n"            \
  "LEDControl::mode_add from your code."

namespace kaleidoscope {
namespace plugin {

class LEDMode;

class LEDControl : public kaleidoscope::Plugin {
 public:
  LEDControl(void);

  static void next_mode(void);
  static void prev_mode(void);
  static void setup(void);
  static void update(void) {
    if (!Kaleidoscope.has_leds)
      return;

    cur_led_mode_->update();
  }
  static void refreshAt(byte row, byte col) {
    if (!Kaleidoscope.has_leds)
      return;

    cur_led_mode_->refreshAt(row, col);
  }
  static void set_mode(uint8_t mode_id);
  static uint8_t get_mode_index() {
    return mode_id;
  }
  static LEDMode *get_mode() {
    return cur_led_mode_;
  }
  template<typename LEDMode__>
  static LEDMode__ *get_mode() {
    return static_cast<LEDMode__*>(cur_led_mode_);
  }

  static void refreshAll() {
    if (!Kaleidoscope.has_leds)
      return;

    if (paused)
      return;

    set_all_leds_to({0, 0, 0});

    cur_led_mode_->onActivate();
  }

  DEPRECATED(LED_CONTROL_MODE_ADD)
  static int8_t mode_add(LEDMode *mode) {
    return 0;
  }

  static void setCrgbAt(int8_t i, cRGB crgb);
  static void setCrgbAt(byte row, byte col, cRGB color);
  static cRGB getCrgbAt(int8_t i);
  static void syncLeds(void);

  static void set_all_leds_to(uint8_t r, uint8_t g, uint8_t b);
  static void set_all_leds_to(cRGB color);

  // We restict activate to LEDModeInterface to make sure that
  // a compiler error is thrown when activate() is accidentally
  // applied to a non-LED mode plugin.
  //
  static void activate(LEDModeInterface *plugin);

  static uint8_t syncDelay;
  static bool paused;

  kaleidoscope::EventHandlerResult onSetup();
  kaleidoscope::EventHandlerResult onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState);
  kaleidoscope::EventHandlerResult beforeReportingState();

 private:
  static uint16_t syncTimer;
  static uint8_t mode_id;
  static uint8_t num_led_modes_;
  static LEDMode *cur_led_mode_;
};

class FocusLEDCommand : public Plugin {
 public:
  FocusLEDCommand() {}

  EventHandlerResult onFocusEvent(const char *command);
};

}

}

extern kaleidoscope::plugin::LEDControl LEDControl;
extern kaleidoscope::plugin::FocusLEDCommand FocusLEDCommand;
