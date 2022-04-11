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

#include "testing/SystemControlReport.h"

#include <cstring>  // for memcpy

#include "kaleidoscope/Runtime.h"  // for Runtime, Runtime_

namespace kaleidoscope {
namespace testing {

SystemControlReport::SystemControlReport(const void *data) {
  const ReportData &report_data =
    *static_cast<const ReportData *>(data);
  memcpy(&report_data_, &report_data, sizeof(report_data_));
  if (report_data_.key != 0) {
    this->push_back(report_data_.key);
  }
  timestamp_ = Runtime.millisAtCycleStart();
}

uint32_t SystemControlReport::Timestamp() const {
  return timestamp_;
}

uint8_t SystemControlReport::ActiveKeycode() const {
  return report_data_.key;
}

}  // namespace testing
}  // namespace kaleidoscope
