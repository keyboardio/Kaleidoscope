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

#include "kaleidoscope/plugin/LEDControl.h"

#include <Arduino.h>                   // for PSTR, strncmp_P
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial

#include "kaleidoscope/KeyAddrMap.h"               // for KeyAddrMap<>::Iterator, KeyAddrMap
#include "kaleidoscope/KeyEvent.h"                 // for KeyEvent
#include "kaleidoscope/KeyMap.h"                   // for KeyMap
#include "kaleidoscope/LiveKeys.h"                 // for LiveKeys, live_keys
#include "kaleidoscope/hooks.h"                    // for Hooks
#include "kaleidoscope/keyswitch_state.h"          // for keyToggledOn
#include "kaleidoscope_internal/LEDModeManager.h"  // for LEDModeManager, LEDModeFactory

using namespace kaleidoscope::internal;  // NOLINT(build/namespaces)

namespace kaleidoscope {
namespace plugin {

static constexpr uint8_t uninitialized_mode_id = 255;

uint8_t LEDControl::mode_id_       = uninitialized_mode_id;
uint8_t LEDControl::num_led_modes_ = LEDModeManager::numLEDModes();
LEDMode *LEDControl::cur_led_mode_ = nullptr;
bool LEDControl::enabled_          = true;

LEDControl::LEDControl(void) {
}
uint8_t LEDControl::sync_interval_   = 32;
uint16_t LEDControl::last_sync_time_ = 0;

void LEDControl::next_mode() {
  ++mode_id_;

  if (mode_id_ >= num_led_modes_) {
    return set_mode(0);
  }

  return set_mode(mode_id_);
}

void LEDControl::prev_mode() {
  if (mode_id_ == 0) {
    // wrap around
    mode_id_ = num_led_modes_ - 1;
  } else {
    mode_id_--;
  }

  return set_mode(mode_id_);
}

void LEDControl::set_mode(uint8_t mode_) {
  if (mode_ >= num_led_modes_)
    return;

  mode_id_ = mode_;

  // Cache the LED mode
  //
  cur_led_mode_ = LEDModeManager::getLEDMode(mode_id_);

  refreshAll();

  Hooks::onLEDModeChange();
}

void LEDControl::activate(LEDModeInterface *plugin) {
  for (uint8_t i = 0; i < num_led_modes_; i++) {

    led_mode_management::LEDModeFactory fac;

    LEDModeManager::retreiveLEDModeFactoryFromPROGMEM(i, fac);

    if (fac.isAssociatedWithPlugin(plugin)) {
      set_mode(i);
      return;
    }
  }
}

void LEDControl::set_all_leds_to(uint8_t r, uint8_t g, uint8_t b) {
  if (!Runtime.has_leds)
    return;

  cRGB color;
  color.r = r;
  color.g = g;
  color.b = b;
  set_all_leds_to(color);
}

void LEDControl::set_all_leds_to(cRGB color) {
  for (auto led_index : Runtime.device().LEDs().all()) {
    setCrgbAt(led_index.offset(), color);
  }
}

void LEDControl::setCrgbAt(uint8_t led_index, cRGB crgb) {
  Runtime.device().setCrgbAt(led_index, crgb);
}

void LEDControl::setCrgbAt(KeyAddr key_addr, cRGB color) {
  Runtime.device().setCrgbAt(key_addr, color);
}

cRGB LEDControl::getCrgbAt(uint8_t led_index) {
  return Runtime.device().getCrgbAt(led_index);
}
cRGB LEDControl::getCrgbAt(KeyAddr key_addr) {
  return Runtime.device().getCrgbAt(Runtime.device().getLedIndex(key_addr));
}

void LEDControl::syncLeds(void) {
  if (!enabled_)
    return;

  // This would be a good spot to introduce a new hook function so that a plugin
  // that needs to override the color of an LED used by an LED mode can do so
  // efficiently.
  Hooks::beforeSyncingLeds();

  Runtime.device().syncLeds();
}

EventHandlerResult LEDControl::onSetup() {
  set_all_leds_to({0, 0, 0});

  LEDModeManager::setupPersistentLEDModes();

  if (mode_id_ == uninitialized_mode_id) {
    set_mode(0);
  }

  return EventHandlerResult::OK;
}

void LEDControl::disable() {
  set_all_leds_to(CRGB(0, 0, 0));
  Runtime.device().syncLeds();
  enabled_ = false;
}

void LEDControl::enable() {
  enabled_ = true;
  refreshAll();
  Runtime.device().syncLeds();
}

EventHandlerResult LEDControl::onKeyEvent(KeyEvent &event) {
  if (event.key.getFlags() != (SYNTHETIC | IS_INTERNAL | LED_TOGGLE))
    return EventHandlerResult::OK;

  if (keyToggledOn(event.state)) {
    if (event.key == Key_LEDEffectNext || event.key == Key_LEDEffectPrevious) {
      // First, check for an active shift key.
      bool shift_active = false;
      // This change should be back-ported to #904
      for (Key active_key : live_keys.all()) {
        if (active_key.isKeyboardShift()) {
          shift_active = true;
          break;
        }
      }
      // Next, record which key (next or previous) was pressed as a boolean.
      bool key_is_next = (event.key == Key_LEDEffectNext);
      // This is basically an XOR with two booleans. If the "next" key was
      // pressed and no shift key is active, or if the "previous" key was
      // pressed and a shift key is active, we activate the next LED
      // Mode. Otherwise, we activate the previous mode.
      if (key_is_next != shift_active) {
        next_mode();
      } else {
        prev_mode();
      }
    } else if (event.key == Key_LEDToggle) {
      if (enabled_)
        disable();
      else
        enable();
    }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult LEDControl::afterEachCycle() {
  if (!enabled_)
    return EventHandlerResult::OK;

  if (Runtime.hasTimeExpired(last_sync_time_, sync_interval_)) {
    syncLeds();
    last_sync_time_ += sync_interval_;
    update();
  }

  return EventHandlerResult::OK;
}

EventHandlerResult FocusLEDCommand::onFocusEvent(const char *input) {
  enum {
    SETALL,
    MODE,
    AT,
    THEME,
    BRIGHTNESS,
  } subCommand;

  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  const char *cmd_at         = PSTR("led.at");
  const char *cmd_setAll     = PSTR("led.setAll");
  const char *cmd_mode       = PSTR("led.mode");
  const char *cmd_brightness = PSTR("led.brightness");
  const char *cmd_theme      = PSTR("led.theme");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_at,
                             cmd_setAll,
                             cmd_mode,
                             cmd_brightness,
                             cmd_theme);

  if (::Focus.inputMatchesCommand(input, cmd_at))
    subCommand = AT;
  else if (::Focus.inputMatchesCommand(input, cmd_setAll))
    subCommand = SETALL;
  else if (::Focus.inputMatchesCommand(input, cmd_mode))
    subCommand = MODE;
  else if (::Focus.inputMatchesCommand(input, cmd_theme))
    subCommand = THEME;
  else if (::Focus.inputMatchesCommand(input, cmd_brightness))
    subCommand = BRIGHTNESS;
  else
    return EventHandlerResult::OK;

  switch (subCommand) {
  case AT: {
    uint8_t idx;

    ::Focus.read(idx);

    if (::Focus.isEOL()) {
      cRGB c = ::LEDControl.getCrgbAt(idx);

      ::Focus.send(c);
    } else {
      cRGB c;

      ::Focus.read(c);

      ::LEDControl.setCrgbAt(idx, c);
    }
    break;
  }
  case BRIGHTNESS: {
    if (::Focus.isEOL()) {
      ::Focus.send(::LEDControl.getBrightness());
    } else {
      uint8_t brightness;

      ::Focus.read(brightness);
      ::LEDControl.setBrightness(brightness);
    }
    break;
  }
  case SETALL: {
    cRGB c;

    ::Focus.read(c);

    ::LEDControl.set_all_leds_to(c);

    break;
  }
  case MODE: {
    char peek = ::Focus.peek();
    if (peek == '\n') {
      ::Focus.send(::LEDControl.get_mode_index());
    } else if (peek == 'n') {
      ::LEDControl.next_mode();
    } else if (peek == 'p') {
      ::LEDControl.prev_mode();
    } else {
      uint8_t mode_id_;

      ::Focus.read(mode_id_);
      ::LEDControl.set_mode(mode_id_);
    }
    break;
  }
  case THEME: {
    if (::Focus.isEOL()) {
      for (auto led_index : Runtime.device().LEDs().all()) {
        cRGB c = ::LEDControl.getCrgbAt(led_index.offset());

        ::Focus.send(c);
      }
      break;
    }

    for (auto led_index : Runtime.device().LEDs().all()) {
      if (::Focus.isEOL()) {
        break;
      }

      cRGB color;

      ::Focus.read(color);

      ::LEDControl.setCrgbAt(led_index.offset(), color);
    }
    break;
  }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDControl LEDControl;
kaleidoscope::plugin::FocusLEDCommand FocusLEDCommand;
