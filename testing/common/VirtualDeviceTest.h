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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#pragma once

#include "./Logging.h"

#include "HID-Settings.h"

namespace kaleidoscope {
namespace testing {

class VirtualDeviceTest : public ::testing::Test {
 protected:
  void SetUp() {
    HIDReportObserver
  }

  void Press(uint8_t row, uint8_t, col) {
    Kaleidoscope.device().keyScanner().setKeystate(
        KeyAddr{row, col},
        Kaleidoscope::Device::Props::keyScanner::KeyState::Pressed);
  }

 private:
  template <class R>
  void ProcessReport(const R& report) {
    observer_RecordReport(report);
  }

  template <class R>
  void RecordReport(const R& report);

  template <>
  void RecordReport<MouseReport>(const MouseReport& report) {
    mouse_reports_.push_back(report);
  }

  template <>
  void RecordReport<BootKeyboardReport>(const BootKeyboardReport& report) {
    boot_keyboard_reports_.push_back(report);
  }

  template <>
  void RecordReport<AbsoluteMouseReport>(const AbsoluteMouseReport& report) {
    absolute_mouse_reports_.push_back(report);
  }

  template <>
  void RecordReport<KeyboardReport>(const KeyboardReport& report) {
    keyboard_reports_.push_back(report);
  }

  std::vector<MouseReport> mouse_reports_;
  std::vector<BootKeyboardReport> boot_keyboard_reports_;
  std::vector<AbsoluteMouseReport> absolute_mouse_reports_;
  std::vector<KeyboardReport> keyboard_reports_;
};

}  // namespace testing
}  // namespace kaleidoscope

#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
