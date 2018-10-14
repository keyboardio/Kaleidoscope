/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#include "kaleidoscope/Kaleidoscope.h"
#include <stdarg.h>

namespace kaleidoscope {

uint32_t Kaleidoscope_::millis_at_cycle_start_;

Kaleidoscope_::Kaleidoscope_(void) {
}

void
Kaleidoscope_::setup(void) {
  kaleidoscope::Hooks::onSetup();

  KeyboardHardware.setup();

  kaleidoscope::hid::initializeKeyboard();
  kaleidoscope::hid::initializeConsumerControl();
  kaleidoscope::hid::initializeSystemControl();

  // A workaround, so that the compiler does not optimize handleKeyswitchEvent out...
  // This is a no-op, but tricks the compiler into not being too helpful
  // TODO(anyone): figure out how to hint the compiler in a more reasonable way
  handleKeyswitchEvent(Key_NoKey, 255, 255, 0);

  // Update the keymap cache, so we start with a non-empty state.
  Layer.updateActiveLayers();
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      Layer.updateLiveCompositeKeymap(row, col);
    }
  }
}

void
Kaleidoscope_::loop(void) {
  millis_at_cycle_start_ = millis();

  kaleidoscope::Hooks::beforeEachCycle();

  KeyboardHardware.scanMatrix();

  kaleidoscope::Hooks::beforeReportingState();

  kaleidoscope::hid::sendKeyboardReport();
  kaleidoscope::hid::releaseAllKeys();

  kaleidoscope::Hooks::afterEachCycle();
}

Kaleidoscope_ Kaleidoscope;

} // namespace kaleidoscope
