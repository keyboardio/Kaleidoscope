/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
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

namespace kaleidoscope {

const MagicCombo::combo_t *MagicCombo::magic_combos;
uint16_t MagicCombo::min_interval = 500;
uint32_t MagicCombo::end_time_;

EventHandlerResult MagicCombo::beforeReportingState() {
  if (!magic_combos)
    return EventHandlerResult::OK;

  for (byte i = 0;; i++) {
    combo_t combo;

    combo.left_hand = pgm_read_dword(&(magic_combos[i].left_hand));
    combo.right_hand = pgm_read_dword(&(magic_combos[i].right_hand));

    if (combo.left_hand == 0 && combo.right_hand == 0)
      break;

    if (LEFTHANDSTATE.all == combo.left_hand &&
        RIGHTHANDSTATE.all == combo.right_hand) {
      if (millis() >= end_time_) {
        magicComboActions(i, combo.left_hand, combo.right_hand);
        end_time_ = millis() + min_interval;
      }
      break;
    }
  }

  return EventHandlerResult::OK;
}

// Legacy V1 API
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void MagicCombo::begin() {
  Kaleidoscope.useLoopHook(legacyLoopHook);
}

void MagicCombo::legacyLoopHook(bool is_post_clear) {
  if (is_post_clear)
    return;
  ::MagicCombo.beforeReportingState();
}
#endif

};

__attribute__((weak)) void magicComboActions(uint8_t comboIndex, uint32_t left_hand, uint32_t right_hand) {
}

kaleidoscope::MagicCombo MagicCombo;
