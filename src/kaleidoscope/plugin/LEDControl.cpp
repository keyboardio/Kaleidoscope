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

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-FocusSerial.h"
#include <Kaleidoscope-EEPROM-Settings.h>
#include "kaleidoscope_internal/LEDModeManager.h"

using namespace kaleidoscope::internal;

namespace kaleidoscope {
namespace plugin {


LEDControl::settings_t LEDControl::settings = {
  .mode_id = 0, // defaults
};

uint8_t LEDControl::num_led_modes_ = LEDModeManager::numLEDModes();
LEDMode *LEDControl::cur_led_mode_;
uint16_t LEDControl::syncDelay = 32;
uint16_t LEDControl::syncTimer;
bool LEDControl::paused = false;
uint16_t LEDControl::settings_base_;

LEDControl::LEDControl(void) {
}

void LEDControl::next_mode(void) {
  settings.mode_id++;

  if (settings.mode_id >= num_led_modes_) {
    return set_mode(0);
  }

  return set_mode(settings.mode_id);
}

void LEDControl::prev_mode(void) {
  if (settings.mode_id == 0) {
    // wrap around
    settings.mode_id = num_led_modes_ - 1;
  } else {
    settings.mode_id--;
  }

  return set_mode(settings.mode_id);
}

void
LEDControl::set_mode(uint8_t mode_) {
  if (mode_ >= num_led_modes_)
    return;

  settings.mode_id = mode_;

  // Cache the LED mode
  //
  cur_led_mode_ = LEDModeManager::getLEDMode(settings.mode_id);

  // store the mode
  KeyboardHardware.storage().put(settings_base_, settings);
  KeyboardHardware.storage().commit();

  refreshAll();
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
  if (!Kaleidoscope.has_leds)
    return;

  cRGB color;
  color.r = r;
  color.g = g;
  color.b = b;
  set_all_leds_to(color);
}

void LEDControl::set_all_leds_to(cRGB color) {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    setCrgbAt(i, color);
  }
}

void LEDControl::setCrgbAt(uint8_t i, cRGB crgb) {
  KeyboardHardware.setCrgbAt(i, crgb);
}

void LEDControl::setCrgbAt(byte row, byte col, cRGB color) {
  KeyboardHardware.setCrgbAt(row, col, color);
}

cRGB LEDControl::getCrgbAt(uint8_t i) {
  return KeyboardHardware.getCrgbAt(i);
}

void LEDControl::syncLeds(void) {
  if (paused)
    return;

  KeyboardHardware.syncLeds();
}

kaleidoscope::EventHandlerResult LEDControl::onSetup() {
  set_all_leds_to({0, 0, 0});

  LEDModeManager::setupPersistentLEDModes();

  syncTimer = millis() + syncDelay;

  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(settings));

  // If mode_id is max, assume that EEPROM is uninitialized, and store the
  // defaults.
  uint8_t mode_id;
  KeyboardHardware.storage().get(settings_base_, mode_id);
  if (mode_id == 0xFF) {
    KeyboardHardware.storage().put(settings_base_, settings);
    KeyboardHardware.storage().commit();
  }

  KeyboardHardware.storage().get(settings_base_, settings);

  set_mode(settings.mode_id);

  return EventHandlerResult::OK;
}

kaleidoscope::EventHandlerResult LEDControl::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | IS_INTERNAL | LED_TOGGLE))
    return kaleidoscope::EventHandlerResult::OK;

  if (keyToggledOn(keyState)) {
    if (mappedKey == Key_LEDEffectNext) {
      next_mode();
    } else if (mappedKey == Key_LEDEffectPrevious) {
      prev_mode();
    }
  }

  return kaleidoscope::EventHandlerResult::EVENT_CONSUMED;
}

kaleidoscope::EventHandlerResult LEDControl::beforeReportingState(void) {
  if (paused)
    return kaleidoscope::EventHandlerResult::OK;

  // unsigned subtraction means that as syncTimer rolls over
  // the same interval is kept
  uint16_t elapsed = Kaleidoscope.millisAtCycleStart() - syncTimer;
  // on some platforms, the subtraction in the comparison results in a signed
  // operation, resulting in syncLeds() no longer getting called.
  if (elapsed > syncDelay) {
    syncLeds();
    syncTimer += syncDelay;
    update();
  }

  return kaleidoscope::EventHandlerResult::OK;
}

EventHandlerResult FocusLEDCommand::onFocusEvent(const char *command) {
  enum {
    SETALL,
    MODE,
    AT,
    THEME,
  } subCommand;

  if (!Kaleidoscope.has_leds)
    return EventHandlerResult::OK;

  if (::Focus.handleHelp(command, PSTR("led.at\n"
                                       "led.setAll\n"
                                       "led.mode\n"
                                       "led.theme")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("led."), 4) != 0)
    return EventHandlerResult::OK;
  if (strcmp_P(command + 4, PSTR("at")) == 0)
    subCommand = AT;
  else if (strcmp_P(command + 4, PSTR("setAll")) == 0)
    subCommand = SETALL;
  else if (strcmp_P(command + 4, PSTR("mode")) == 0)
    subCommand = MODE;
  else if (strcmp_P(command + 4, PSTR("theme")) == 0)
    subCommand = THEME;
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

      ::Focus.read(LEDControl::settings.mode_id);
      ::LEDControl.set_mode(LEDControl::settings.mode_id);
    }
    break;
  }
  case THEME: {
    if (::Focus.isEOL()) {
      for (uint8_t idx = 0; idx < LED_COUNT; idx++) {
        cRGB c = ::LEDControl.getCrgbAt(idx);

        ::Focus.send(c);
      }
      break;
    }

    int8_t idx = 0;
    while (idx < LED_COUNT && !::Focus.isEOL()) {
      cRGB color;

      ::Focus.read(color);

      ::LEDControl.setCrgbAt(idx, color);
      idx++;
    }
    break;
  }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::LEDControl LEDControl;
kaleidoscope::plugin::FocusLEDCommand FocusLEDCommand;
