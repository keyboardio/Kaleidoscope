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

#include "testing/ConsumerControlReport.h"

#include <cstring>  // for memcpy
#include <vector>   // for vector

#include "kaleidoscope/Runtime.h"  // for Runtime, Runtime_

namespace kaleidoscope {
namespace testing {

ConsumerControlReport::ConsumerControlReport(const void *data) {
  const ReportData &report_data =
    *static_cast<const ReportData *>(data);
  memcpy(&report_data_, &report_data, sizeof(report_data_));
  timestamp_ = Runtime.millisAtCycleStart();
}

uint32_t ConsumerControlReport::Timestamp() const {
  return timestamp_;
}

std::vector<uint16_t> ConsumerControlReport::ActiveKeycodes() const {
  std::vector<uint16_t> keys;
  if (report_data_.key1) keys.push_back(report_data_.key1);
  if (report_data_.key2) keys.push_back(report_data_.key2);
  if (report_data_.key3) keys.push_back(report_data_.key3);
  if (report_data_.key4) keys.push_back(report_data_.key4);
  return keys;
}

}  // namespace testing
}  // namespace kaleidoscope
