/* Kaleidoscope-NumPad - A NumPad plugin for Kaleidoscope.
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

#include "Kaleidoscope-NumPad.h"
#include "LEDUtils.h"
#include "Kaleidoscope.h"
#include "layers.h"

byte NumPad_::numpadLayerToggleKeyRow = 255, NumPad_::numpadLayerToggleKeyCol = 255;
uint8_t NumPad_::numPadLayer;
bool NumPad_::numlockUnsynced = false;
bool NumPad_::originalNumLockState = false;
cRGB NumPad_::color = CRGB(160, 0, 0);
uint8_t NumPad_::lock_hue = 170;

kaleidoscope::EventHandlerResult NumPad_::onSetup(void) {
  originalNumLockState = getNumlockState();
  return kaleidoscope::EventHandlerResult::OK;
}

bool NumPad_::getNumlockState() {
  return !!(kaleidoscope::hid::getKeyboardLEDs() & LED_NUM_LOCK);
}

void NumPad_::syncNumlockState(bool state) {
  bool numLockLEDState = getNumlockState();
  if (numLockLEDState != state) {
    kaleidoscope::hid::pressKey(Key_KeypadNumLock);
  }
}



void NumPad_::cleanupNumlockState() {
  if (!numlockUnsynced) {
    bool numLockLEDState = getNumlockState();
    LEDControl.set_mode(LEDControl.get_mode_index());
    if (!originalNumLockState) {
      syncNumlockState(false);
      numLockLEDState = false;
    }
    originalNumLockState = numLockLEDState;
    numlockUnsynced = true;
  }

}

void NumPad_::setKeyboardLEDColors(void) {
  LEDControl.set_mode(LEDControl.get_mode_index());

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);
      Key layer_key = Layer.getKey(numPadLayer, r, c);

      if (k == LockLayer(numPadLayer)) {
        numpadLayerToggleKeyRow = r;
        numpadLayerToggleKeyCol = c;
      }

      if ((k != layer_key) || (k == Key_NoKey) || (k.flags != KEY_FLAGS)) {
        LEDControl.refreshAt(r, c);
      } else {
        LEDControl.setCrgbAt(r, c, color);
      }
    }
  }

  if ((numpadLayerToggleKeyRow <= ROWS) && (numpadLayerToggleKeyCol <= COLS)) {


    cRGB lock_color = breath_compute(lock_hue);
    LEDControl.setCrgbAt(numpadLayerToggleKeyRow, numpadLayerToggleKeyCol, lock_color);
  }
}

kaleidoscope::EventHandlerResult NumPad_::afterEachCycle() {
  if (!Layer.isOn(numPadLayer)) {
    cleanupNumlockState();
  } else {
    if (numlockUnsynced)  {
      // If it's the first time we're in this loop after toggling the Numpad mode on
      syncNumlockState(true);
      numlockUnsynced = false;
    }
    setKeyboardLEDColors();
  }
  return kaleidoscope::EventHandlerResult::OK;
}



NumPad_ NumPad;
