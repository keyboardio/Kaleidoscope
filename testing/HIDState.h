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

// IWYU pragma: no_include <__memory/unique_ptr.h>

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint8_t
#include <memory>    // IWYU pragma: keep
#include <vector>    // for vector

#include "testing/AbsoluteMouseReport.h"    // for AbsoluteMouseReport
#include "testing/ConsumerControlReport.h"  // for ConsumerControlReport
#include "testing/KeyboardReport.h"         // for KeyboardReport
#include "testing/MouseReport.h"            // for MouseReport
#include "testing/SystemControlReport.h"    // for SystemControlReport

namespace kaleidoscope {
namespace testing {
namespace internal {
class HIDStateBuilder;
}

class HIDState {
 public:
  const std::vector<AbsoluteMouseReport> &AbsoluteMouse() const;
  const AbsoluteMouseReport &AbsoluteMouse(size_t i) const;

  const std::vector<ConsumerControlReport> &ConsumerControl() const;
  const ConsumerControlReport &ConsumerControl(size_t i) const;

  const std::vector<KeyboardReport> &Keyboard() const;
  const KeyboardReport &Keyboard(size_t i) const;

  const std::vector<MouseReport> &Mouse() const;
  const MouseReport &Mouse(size_t i) const;

  const std::vector<SystemControlReport> &SystemControl() const;
  const SystemControlReport &SystemControl(size_t i) const;

 private:
  friend class internal::HIDStateBuilder;

  std::vector<AbsoluteMouseReport> absolute_mouse_reports_;
  std::vector<ConsumerControlReport> consumer_control_reports_;
  std::vector<KeyboardReport> keyboard_reports_;
  std::vector<MouseReport> mouse_reports_;
  std::vector<SystemControlReport> system_control_reports_;
};

namespace internal {

class HIDStateBuilder {
 public:
  static void ProcessHidReport(
    uint8_t id, const void *data, int len, int result);

  static std::unique_ptr<HIDState> Snapshot();

 private:
  static void Clear();
  static void ProcessAbsoluteMouseReport(const AbsoluteMouseReport &report);
  static void ProcessConsumerControlReport(const ConsumerControlReport &report);
  static void ProcessKeyboardReport(const KeyboardReport &report);
  static void ProcessMouseReport(const MouseReport &report);
  static void ProcessSystemControlReport(const SystemControlReport &report);

  static std::vector<AbsoluteMouseReport> absolute_mouse_reports_;
  static std::vector<ConsumerControlReport> consumer_control_reports_;
  static std::vector<KeyboardReport> keyboard_reports_;
  static std::vector<MouseReport> mouse_reports_;
  static std::vector<SystemControlReport> system_control_reports_;
};

}  // namespace internal
}  // namespace testing
}  // namespace kaleidoscope
