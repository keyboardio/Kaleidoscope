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
#include "testing/common/KeyboardReport.h"
#include "testing/common/SystemControlReport.h"

// Out of order due to macro conflicts.
#include "testing/common/fix-macros.h"
#include <memory>

namespace kaleidoscope {
namespace testing {

class State {
 public:
  static void ProcessHidReport(
      uint8_t id, const void* data, int len, int result);

  static std::unique_ptr<State> Snapshot();

  const std::vector<KeyboardReport>& KeyboardReports() const;
  const KeyboardReport& KeyboardReports(size_t i) const;

  const std::vector<SystemControlReport>& SystemControlReports() const;
  const SystemControlReport& SystemControlReports(size_t i) const;

 private:
  static std::vector<KeyboardReport> keyboard_reports;
  static std::vector<SystemControlReport> system_control_reports;

  static void Clear();
  static void ProcessKeyboardReport(const KeyboardReport& report);
  static void ProcessSystemControlReport(const SystemControlReport& report);

  std::vector<KeyboardReport> keyboard_reports_;
  std::vector<SystemControlReport> system_control_reports_;
};

}  // namespace testing
}  // namespace kaleidoscope
