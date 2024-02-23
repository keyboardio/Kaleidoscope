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

#include "testing/AbsoluteMouseReport.h"

#include <cstring>  // for memcpy
#include <vector>   // for vector

#include "kaleidoscope/MouseButtons.h"  // for MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_NEXT, MOUSE_PREV, MOUS...
#include "kaleidoscope/Runtime.h"       // for Runtime, Runtime_

namespace kaleidoscope {
namespace testing {

AbsoluteMouseReport::AbsoluteMouseReport(const void *data) {
  const ReportData &report_data =
    *static_cast<const ReportData *>(data);
  memcpy(&report_data_, &report_data, sizeof(report_data_));
  timestamp_ = Runtime.millisAtCycleStart();
}

uint32_t AbsoluteMouseReport::Timestamp() const {
  return timestamp_;
}

std::vector<uint8_t> AbsoluteMouseReport::Buttons() const {
  std::vector<uint8_t> buttons;
  const uint8_t bs = report_data_.buttons;
  if (bs & MOUSE_LEFT) buttons.push_back(MOUSE_LEFT);
  if (bs & MOUSE_RIGHT) buttons.push_back(MOUSE_RIGHT);
  if (bs & MOUSE_MIDDLE) buttons.push_back(MOUSE_MIDDLE);
  if (bs & MOUSE_PREV) buttons.push_back(MOUSE_PREV);
  if (bs & MOUSE_NEXT) buttons.push_back(MOUSE_NEXT);
  return buttons;
}

uint16_t AbsoluteMouseReport::XAxis() const {
  return report_data_.xAxis;
}

uint16_t AbsoluteMouseReport::YAxis() const {
  return report_data_.yAxis;
}

int8_t AbsoluteMouseReport::Wheel() const {
  return report_data_.wheel;
}

}  // namespace testing
}  // namespace kaleidoscope
