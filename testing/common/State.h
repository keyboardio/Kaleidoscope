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

#include <cstddef>
#include <cstdint>
#include <vector>

#include "HID-Settings.h"
#include "HIDReportObserver.h"
#include "testing/common/KeyboardReport.h"

namespace kaleidoscope {
namespace testing {

class State {
 public:
  ~State();

  static void DefaultProcessHidReport(uint8_t, const void*, int, int);

  void ProcessHidReport(uint8_t id, const void* data, int len, int result);

  const std::vector<KeyboardReport>& KeyboardReports() const;
  const KeyboardReport& KeyboardReports(size_t i) const;

 private:
  friend class VirtualDeviceTest;

  State();

  void ProcessKeyboardReport(const KeyboardReport& report);

  void Snapshot();

  std::vector<KeyboardReport> keyboard_reports_;
};

}  // namespace testing
}  // namespace kaleidoscope
