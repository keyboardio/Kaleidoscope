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

namespace kaleidoscope {
namespace plugin {

uint16_t Turbo::interval_ = 10;
uint16_t Turbo::flashInterval_ = 69;
bool Turbo::sticky_ = false;
bool Turbo::flash_ = true;
cRGB Turbo::activeColor_ = CRGB(160, 0, 0);

bool Turbo::enable = false;
uint32_t Turbo::startTime = 0;
uint32_t Turbo::flashStartTime = 0;
byte Turbo::keyPositions[4];
uint16_t Turbo::numKeys = 0;

uint16_t Turbo::interval() {
  return interval_;
}
void Turbo::interval(uint16_t newVal) {
  interval_ = newVal;
}

uint16_t Turbo::flashInterval() {
  return flashInterval_;
}
void Turbo::flashInterval(uint16_t newVal) {
  flashInterval_ = newVal;
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
  return activeColor_;
}
void Turbo::activeColor(cRGB newVal) {
  activeColor_ = newVal;
}

void Turbo::findKeyPositions() {
  numKeys = 0;
  for (byte r = 0; r < ROWS; r++) {
    for (byte c = 0; c < COLS; c++) {
      if (Layer.lookupOnActiveLayer(r, c) == Key_Turbo) {
        keyPositions[numKeys++] = r * COLS + c;
      }
    }
  }
}

EventHandlerResult Turbo::onSetup() {
  Turbo::findKeyPositions();
  return EventHandlerResult::OK;
}

EventHandlerResult Turbo::onLayerChange() {
  Turbo::findKeyPositions();
  return EventHandlerResult::OK;
}

EventHandlerResult Turbo::onKeyswitchEvent(Key &key, byte row, byte col, uint8_t key_state) {
  if (key != Key_Turbo) return EventHandlerResult::OK;
  enable = sticky_ ? (keyIsPressed(key_state) ? enable : !enable) : keyIsPressed(key_state);
  if (!enable) {
    for (uint16_t i = 0; i < numKeys; i++) {
      LEDControl::refreshAt(keyPositions[i] / COLS, keyPositions[i] % COLS);
    }
  }
  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult Turbo::afterEachCycle() {
  if (enable) {
    if (Kaleidoscope.millisAtCycleStart() - startTime > interval_) {
      kaleidoscope::hid::sendKeyboardReport();
      startTime = Kaleidoscope.millisAtCycleStart();
    }

    if (flash_) {
      if (Kaleidoscope.millisAtCycleStart() - flashStartTime > flashInterval_ * 2) {
        for (uint16_t i = 0; i < numKeys; i++) {
          LEDControl::setCrgbAt(keyPositions[i] / COLS, keyPositions[i] % COLS, activeColor_);
        }
        flashStartTime = Kaleidoscope.millisAtCycleStart();
      } else if (Kaleidoscope.millisAtCycleStart() - flashStartTime > flashInterval_) {
        for (uint16_t i = 0; i < numKeys; i++) {
          LEDControl::setCrgbAt(keyPositions[i] / COLS, keyPositions[i] % COLS, {0, 0, 0});
        }
      }
      LEDControl::syncLeds();
    } else {
      for (uint16_t i = 0; i < numKeys; i++) {
        LEDControl::setCrgbAt(keyPositions[i] / COLS, keyPositions[i] % COLS, activeColor_);
      }
    }
  }
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::Turbo Turbo;
