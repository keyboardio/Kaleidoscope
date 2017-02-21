/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-MagicCombo.h>

#if defined(ARDUINO_AVR_MODEL01)
#define LEFTHANDSTATE  KeyboardHardware.leftHandState
#define RIGHTHANDSTATE KeyboardHardware.rightHandState
#endif

#if defined(ARDUINO_AVR_SHORTCUT)
#define LEFTHANDSTATE  KeyboardHardware.scanner.leftHandState
#define RIGHTHANDSTATE KeyboardHardware.scanner.rightHandState
#endif

namespace KaleidoscopePlugins {

  const MagicCombo::dictionary_t *MagicCombo::dictionary;
  uint16_t MagicCombo::minInterval = 500;
  uint32_t MagicCombo::endTime;

  MagicCombo::MagicCombo (void) {
  }

  void
  MagicCombo::begin (void) {
    loop_hook_use (this->loopHook);
  }

  void
  MagicCombo::configure (const MagicCombo::dictionary_t dictionary_[]) {
    dictionary = (dictionary_t *)dictionary_;
  }

  void
  MagicCombo::loopHook (bool postClear) {
    if (!dictionary || postClear)
      return;

    for (byte i = 0;; i++) {
      dictionary_t combo;

      combo.leftHand = pgm_read_dword (&(dictionary[i].leftHand));
      combo.rightHand = pgm_read_dword (&(dictionary[i].rightHand));

      if (combo.leftHand == 0 && combo.rightHand == 0)
        break;

      if (LEFTHANDSTATE.all == combo.leftHand &&
          RIGHTHANDSTATE.all == combo.rightHand) {
        if (millis () >= endTime) {
          magicComboActions (i, combo.leftHand, combo.rightHand);
          endTime = millis () + minInterval;
        }
        break;
      }
    }
  }

};

__attribute__((weak))
void
magicComboActions (uint8_t comboIndex, uint32_t leftHand, uint32_t rightHand) {
}

KaleidoscopePlugins::MagicCombo MagicCombo;
