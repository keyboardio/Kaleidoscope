/* -*- mode: c++ -*-
 * Copyright (C) 2020  Eric Paniagua (epaniagua@google.com)
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

#include "testing/KeyboardReport.h"

#include <cstring>  // for memcpy
#include <vector>   // for vector<>::iterator, vector

#include "kaleidoscope/Runtime.h"   // for Runtime, Runtime_
#include "kaleidoscope/key_defs.h"  // for HID_KEYBOARD_FIRST_MODIFIER, HID_LAST_KEY

namespace kaleidoscope {
namespace testing {

KeyboardReport::KeyboardReport(const void *data) {
  const ReportData &report_data =
    *static_cast<const ReportData *>(data);
  memcpy(&report_data_, &report_data, sizeof(report_data_));
  timestamp_ = Runtime.millisAtCycleStart();
}

uint32_t KeyboardReport::Timestamp() const {
  return timestamp_;
}

std::vector<uint8_t> KeyboardReport::ActiveKeycodes() const {
  auto keycodes = ActiveNonModifierKeycodes();
  auto mods     = ActiveModifierKeycodes();
  keycodes.insert(keycodes.end(), mods.begin(), mods.end());
  return keycodes;
}

std::vector<uint8_t> KeyboardReport::ActiveModifierKeycodes() const {
  constexpr uint8_t modifier_keycode_offset{HID_KEYBOARD_FIRST_MODIFIER};

  std::vector<uint8_t> active_modifiers;

  uint8_t modifiers{report_data_.modifiers}, mask{1};

  for (uint8_t i{0}; modifiers != 0; ++i, modifiers >>= 1) {
    if (modifiers & mask) {
      active_modifiers.push_back(i + modifier_keycode_offset);
    }
  }

  return active_modifiers;
}

std::vector<uint8_t> KeyboardReport::ActiveNonModifierKeycodes() const {
  std::vector<uint8_t> active_keycodes;

  for (uint8_t i = 0; i < HID_LAST_KEY; ++i) {
    uint8_t bit     = 1 << (uint8_t(i) % 8);
    uint8_t keycode = report_data_.nkro_keys[i / 8] & bit;
    if (keycode) active_keycodes.push_back(i);
  }

  return active_keycodes;
}

}  // namespace testing
}  // namespace kaleidoscope
