/* Kaleidoscope-LEDControl - LED control plugin for Kaleidoscope
 * Copyright (C) 2017-2020  Keyboard.io, Inc.
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

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/plugin/LEDMode.h"

#define LED_TOGGLE   B00000001  // Synthetic, internal

#define Key_LEDEffectNext Key(0, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE)
#define Key_LEDEffectPrevious Key(1, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE)
#define Key_LEDToggle Key(2, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE)

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
    if (!Runtime.has_leds)
      return;

    cur_led_mode_->update();
  }
  static void refreshAt(KeyAddr key_addr) {
    if (!Runtime.has_leds)
      return;

    cur_led_mode_->refreshAt(key_addr);
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
  DEPRECATED(ROW_COL_FUNC) static void refreshAt(byte row, byte col) {
    refreshAt(KeyAddr(row, col));
  }

  static void refreshAll() {
    if (!Runtime.has_leds)
      return;

    if (!enabled_)
      return;

    set_all_leds_to({0, 0, 0});

    cur_led_mode_->onActivate();
  }

  DEPRECATED(LED_CONTROL_MODE_ADD)
  static int8_t mode_add(LEDMode *mode) {
    return 0;
  }

  static void setCrgbAt(uint8_t led_index, cRGB crgb);
  static void setCrgbAt(KeyAddr key_addr, cRGB color);
  DEPRECATED(ROW_COL_FUNC) static void setCrgbAt(byte row, byte col, cRGB color) {
    setCrgbAt(KeyAddr(row, col), color);
  }
  static cRGB getCrgbAt(uint8_t led_index);
  static cRGB getCrgbAt(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) static cRGB getCrgbAt(byte row, byte col) {
    return getCrgbAt(KeyAddr(row, col));
  }
  static void syncLeds(void);

  static void set_all_leds_to(uint8_t r, uint8_t g, uint8_t b);
  static void set_all_leds_to(cRGB color);

  // We restict activate to LEDModeInterface to make sure that
  // a compiler error is thrown when activate() is accidentally
  // applied to a non-LED mode plugin.
  //
  static void activate(LEDModeInterface *plugin);

  static uint8_t syncDelay;

  kaleidoscope::EventHandlerResult onSetup();
  kaleidoscope::EventHandlerResult onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState);
  kaleidoscope::EventHandlerResult beforeReportingState();

  static void disable();
  static void enable();
  static bool isEnabled() {
    return enabled_;
  }

  static void setBrightness(uint8_t brightness) {
    Runtime.device().ledDriver().setBrightness(brightness);
  }
  static uint8_t getBrightness() {
    return Runtime.device().ledDriver().getBrightness();
  }

  // The data proxy objects are required to only emit deprecation
  // messages when the `paused` property is accessed directly.
  //
  // Once the deprecation period elapsed, the proxy class and the proxied
  // `paused` property can be safely removed.
  class DataProxy {
   public:
    DataProxy() = default;

    //constexpr DataProxy(bool value) : value_{value} {}

    DEPRECATED(DIRECT_LEDCONTROL_PAUSED_ACCESS)
    DataProxy &operator=(bool value) {
      if (value)
        disable();
      else
        enable();
      return *this;
    }

    DEPRECATED(DIRECT_LEDCONTROL_PAUSED_ACCESS)
    operator bool () const {
      return !isEnabled();
    }
  };

  DataProxy paused;

 private:
  static uint16_t syncTimer;
  static uint8_t mode_id;
  static uint8_t num_led_modes_;
  static LEDMode *cur_led_mode_;
  static bool enabled_;
  static Key pending_next_prev_key_;
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
