/* -*- mode: c++ -*-
 * Copyright (C) 2020  Eric Paniagua (epaniagua@google.com)
 * Copyright (C) 2024  Keyboard.io, Inc
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

#include <cstdint>  // for uint8_t, int8_t, uint32_t

#include "HID-Settings.h"                           // for HID_REPORTID_MOUSE
#include "kaleidoscope/MouseButtons.h"              // for MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_NEXT, MOUSE_PREV, MOUSE_R...
#include "kaleidoscope/driver/hid/apis/MouseAPI.h"  // for HID_MouseReport_Data_t

namespace kaleidoscope {
namespace testing {

class MouseReport {
 public:
  typedef HID_MouseReport_Data_t ReportData;

  static constexpr uint8_t kHidReportType = HID_REPORTID_MOUSE;

  MouseReport(const void *data);


  static constexpr uint8_t kButtonLeft   = MOUSE_LEFT;
  static constexpr uint8_t kButtonRight  = MOUSE_RIGHT;
  static constexpr uint8_t kButtonMiddle = MOUSE_MIDDLE;
  static constexpr uint8_t kButtonPrev   = MOUSE_PREV;
  static constexpr uint8_t kButtonNext   = MOUSE_NEXT;

  uint32_t Timestamp() const;
  uint8_t Buttons() const;
  int8_t XAxis() const;
  int8_t YAxis() const;
  int8_t VWheel() const;
  int8_t HWheel() const;

 private:
  uint32_t timestamp_;
  ReportData report_data_;
};

}  // namespace testing
}  // namespace kaleidoscope
