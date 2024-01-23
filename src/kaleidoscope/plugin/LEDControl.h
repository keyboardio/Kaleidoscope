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

#include <stdint.h>  // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"                  // for KeyAddr
#include "kaleidoscope/KeyEvent.h"                 // for KeyEvent
#include "kaleidoscope/Runtime.h"                  // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"            // for cRGB, Device, Base<>::LEDDriver, Virtu...
#include "kaleidoscope/event_handler_result.h"     // for EventHandlerResult
#include "kaleidoscope/key_defs.h"                 // for Key, IS_INTERNAL, KEY_FLAGS, SYNTHETIC
#include "kaleidoscope/plugin.h"                   // for Plugin
#include "kaleidoscope/plugin/LEDMode.h"           // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"  // for LEDModeInterface

constexpr uint8_t LED_TOGGLE = 0b00000001;  // Synthetic, internal

constexpr Key Key_LEDEffectNext     = Key(0, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE);
constexpr Key Key_LEDEffectPrevious = Key(1, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE);
constexpr Key Key_LEDToggle         = Key(2, KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE);

namespace kaleidoscope {
namespace plugin {

class LEDControl : public kaleidoscope::Plugin {
 public:
  LEDControl(void);

  static void next_mode(void);
  static void prev_mode(void);
  static void setup(void);
  static void update(void) {
    if (!Runtime.has_leds)
      return;

    if (cur_led_mode_ != nullptr)
      cur_led_mode_->update();
  }
  static void refreshAt(KeyAddr key_addr) {
    if (!Runtime.has_leds)
      return;

    if (cur_led_mode_ != nullptr)
      cur_led_mode_->refreshAt(key_addr);
  }
  static void set_mode(uint8_t mode_id);
  static uint8_t get_mode_index() {
    return mode_id_;
  }
  static LEDMode *get_mode() {
    return cur_led_mode_;
  }
  template<typename LEDMode__>
  static LEDMode__ *get_mode() {
    return static_cast<LEDMode__ *>(cur_led_mode_);
  }

  static void refreshAll() {
    if (!Runtime.has_leds)
      return;

    if (!enabled_)
      return;

    set_all_leds_to({0, 0, 0});

    if (cur_led_mode_ != nullptr)
      cur_led_mode_->onActivate();
  }

  static void setCrgbAt(uint8_t led_index, cRGB crgb);
  static void setCrgbAt(KeyAddr key_addr, cRGB color);
  static cRGB getCrgbAt(uint8_t led_index);
  static cRGB getCrgbAt(KeyAddr key_addr);
  static void syncLeds(void);

  static void set_all_leds_to(uint8_t r, uint8_t g, uint8_t b);
  static void set_all_leds_to(cRGB color);

  // We restict activate to LEDModeInterface to make sure that
  // a compiler error is thrown when activate() is accidentally
  // applied to a non-LED mode plugin.
  //
  static void activate(LEDModeInterface *plugin);

  static void setSyncInterval(uint8_t interval) {
    sync_interval_ = interval;
  }

  EventHandlerResult onSetup();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();

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

 private:
  static uint16_t last_sync_time_;
  static uint8_t sync_interval_;
  static uint8_t mode_id_;
  static uint8_t num_led_modes_;
  static LEDMode *cur_led_mode_;
  static bool enabled_;
};


}  // namespace plugin

}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDControl LEDControl;
