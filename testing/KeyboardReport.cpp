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

#include "testing/KeyboardReport.h"

#include <cstring>

namespace kaleidoscope {
namespace testing {

KeyboardReport::KeyboardReport(const void* data) {
  const ReportData& report_data =
    *static_cast<const ReportData*>(data);
  memcpy(&report_data_, &report_data, sizeof(report_data_));
}

std::vector<uint8_t> KeyboardReport::ActiveKeycodes() const {
  std::vector<uint8_t> active_keys;
  for (uint8_t i = 0; i < HID_LAST_KEY; ++i) {
    uint8_t bit = 1 << (uint8_t(i) % 8);
    uint8_t key_code = report_data_.keys[i / 8] & bit;
    if (key_code) active_keys.push_back(i);
  }
  return active_keys;
}

}  // namespace testing
}  // namespace kaleidoscope
