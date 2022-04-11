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

#include "testing/MouseReport.h"

#include <cstring>  // for memcpy

#include "kaleidoscope/Runtime.h"  // for Runtime, Runtime_

namespace kaleidoscope {
namespace testing {

MouseReport::MouseReport(const void *data) {
  const ReportData &report_data =
    *static_cast<const ReportData *>(data);
  memcpy(&report_data_, &report_data, sizeof(report_data_));
  timestamp_ = Runtime.millisAtCycleStart();
}

uint32_t MouseReport::Timestamp() const {
  return timestamp_;
}

uint8_t MouseReport::Buttons() const {
  return report_data_.buttons;
}

int8_t MouseReport::XAxis() const {
  return report_data_.xAxis;
}

int8_t MouseReport::YAxis() const {
  return report_data_.yAxis;
}

int8_t MouseReport::VWheel() const {
  return report_data_.vWheel;
}

int8_t MouseReport::HWheel() const {
  return report_data_.hWheel;
}

}  // namespace testing
}  // namespace kaleidoscope
