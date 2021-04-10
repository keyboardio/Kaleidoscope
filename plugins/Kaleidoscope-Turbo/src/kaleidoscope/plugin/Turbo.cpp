/* -*- mode: c++ -*-
 * Kaleidoscope-Turbo
 * Copyright (C) 2018 ash lea
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

#include <Kaleidoscope-Turbo.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/layers.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

uint16_t Turbo::interval_ = 10;
uint16_t Turbo::flash_interval_ = 69;
bool Turbo::sticky_ = false;
bool Turbo::flash_ = true;
cRGB Turbo::active_color_ = CRGB(160, 0, 0);

bool Turbo::active_ = false;
uint32_t Turbo::start_time_ = 0;
uint32_t Turbo::flash_start_time_ = 0;

uint16_t Turbo::interval() {
  return interval_;
}
void Turbo::interval(uint16_t newVal) {
  interval_ = newVal;
}

uint16_t Turbo::flashInterval() {
  return flash_interval_;
}
void Turbo::flashInterval(uint16_t newVal) {
  flash_interval_ = newVal;
}

bool Turbo::sticky() {
  return sticky_;
}
void Turbo::sticky(bool newVal) {
  sticky_ = newVal;
}

bool Turbo::flash() {
  return flash_;
}
void Turbo::flash(bool newVal) {
  flash_ = newVal;
}

cRGB Turbo::activeColor() {
  return active_color_;
}
void Turbo::activeColor(cRGB newVal) {
  active_color_ = newVal;
}

EventHandlerResult Turbo::onKeyEvent(KeyEvent &event) {
  if (active_ && flash_ && keyToggledOff(event.state)) {
    if (event.key.isKeyboardKey())
      LEDControl::refreshAt(event.addr);
  }

  if (event.key != Key_Turbo)
    return EventHandlerResult::OK;

  if (keyToggledOn(event.state)) {
    active_ = true;
    start_time_ = Runtime.millisAtCycleStart() - interval_;
  } else {
    active_ = false;
    if (flash_)
      LEDControl::refreshAll();
  }
  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult Turbo::afterEachCycle() {
  if (active_) {
    if (Runtime.hasTimeExpired(start_time_, interval_)) {
      // Reset the timer.
      start_time_ = Runtime.millisAtCycleStart();

      // Clear the existing Keyboard HID report. It might be nice to keep the
      // modifiers active, but I'll save that for another time.
      Runtime.hid().keyboard().releaseAllKeys();
      // Send the empty report to register the release of all the held keys.
      Runtime.hid().keyboard().sendReport();

      // Just in case the Turbo key has been wiped from `live_keys[]` without
      // `onKeyEvent()` being called with a toggle-off:
      active_ = false;

      // Go through the `live_keys[]` array and add any Keyboard HID keys to the
      // new report.
      for (Key key : live_keys.all()) {
        if (key == Key_Turbo) {
          active_ = true;
        }
        if (key.isKeyboardKey()) {
          Runtime.addToReport(key);
        }
      }

      // Send the re-populated keyboard report.
      Runtime.hid().keyboard().sendReport();
    }
  }
  return EventHandlerResult::OK;
}

EventHandlerResult Turbo::beforeSyncingLeds() {
  if (flash_ && active_) {
    static bool leds_on = false;
    cRGB color = CRGB(0, 0, 0);
    if (leds_on) {
      color = active_color_;
    }
    if (Runtime.hasTimeExpired(flash_start_time_, flash_interval_)) {
      flash_start_time_ = Runtime.millisAtCycleStart();
      leds_on = !leds_on;
    }
    for (KeyAddr key_addr : KeyAddr::all()) {
      Key key = live_keys[key_addr];
      if (key.isKeyboardKey()) {
        LEDControl::setCrgbAt(key_addr, color);
      }
    }
  }
  return EventHandlerResult::OK;
}

EventHandlerResult Turbo::onNameQuery() {
  return ::Focus.sendName(F("Turbo"));
}

}
}

kaleidoscope::plugin::Turbo Turbo;
