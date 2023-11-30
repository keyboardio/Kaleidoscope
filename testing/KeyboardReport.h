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

#pragma once

#include <cstdint>  // for uint8_t, uint32_t
#include <vector>   // for vector

#include "HID-Settings.h"               // for HID_REPORTID_NKRO_KEYBOARD
#include "BootKeyboard/BootKeyboard.h"  // for HID_KeyboardReport_Data_t

namespace kaleidoscope {
namespace testing {

class KeyboardReport {
 public:
  typedef HID_BootKeyboardReport_Data_t ReportData;

  static constexpr uint8_t kHidReportType = HID_REPORTID_KEYBOARD;

  KeyboardReport(const void *data);

  uint32_t Timestamp() const;
  std::vector<uint8_t> ActiveKeycodes() const;
  std::vector<uint8_t> ActiveModifierKeycodes() const;
  std::vector<uint8_t> ActiveNonModifierKeycodes() const;

 private:
  uint32_t timestamp_;
  ReportData report_data_;
};

}  // namespace testing
}  // namespace kaleidoscope
