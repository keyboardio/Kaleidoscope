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

#include "testing/AbsoluteMouseReport.h"
#include "testing/ConsumerControlReport.h"
#include "testing/KeyboardReport.h"
#include "testing/SystemControlReport.h"

// Out of order due to macro conflicts.
#include "testing/fix-macros.h"
#include <memory>

namespace kaleidoscope {
namespace testing {
namespace internal {
class HIDStateBuilder;
}

class HIDState {
 public:
  const std::vector<AbsoluteMouseReport>& AbsoluteMouse() const;
  const AbsoluteMouseReport& AbsoluteMouse(size_t i) const;

  const std::vector<ConsumerControlReport>& ConsumerControl() const;
  const ConsumerControlReport& ConsumerControl(size_t i) const;

  const std::vector<KeyboardReport>& Keyboard() const;
  const KeyboardReport& Keyboard(size_t i) const;

  const std::vector<SystemControlReport>& SystemControl() const;
  const SystemControlReport& SystemControl(size_t i) const;

 private:
  friend class internal::HIDStateBuilder;

  std::vector<AbsoluteMouseReport> absolute_mouse_reports_;
  std::vector<ConsumerControlReport> consumer_control_reports_;
  std::vector<KeyboardReport> keyboard_reports_;
  std::vector<SystemControlReport> system_control_reports_;
};

namespace internal {

class HIDStateBuilder {
 public:
  static void ProcessHidReport(
    uint8_t id, const void* data, int len, int result);

  static std::unique_ptr<HIDState> Snapshot();

 private:
  static void Clear();
  static void ProcessAbsoluteMouseReport(const AbsoluteMouseReport& report);
  static void ProcessConsumerControlReport(const ConsumerControlReport& report);
  static void ProcessKeyboardReport(const KeyboardReport& report);
  static void ProcessSystemControlReport(const SystemControlReport& report);

  static std::vector<AbsoluteMouseReport> absolute_mouse_reports_;
  static std::vector<ConsumerControlReport> consumer_control_reports_;
  static std::vector<KeyboardReport> keyboard_reports_;
  static std::vector<SystemControlReport> system_control_reports_;
};

}  // namespace internal
}  // namespace testing
}  // namespace kaleidoscope
