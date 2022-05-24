/* -*- mode: c++ -*-
 * Copyright (C) 2022  Keyboard.io, Inc
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

#include <cstdint>  // for int8_t, uint32_t, uint8_t

#include "MouseReport.h"       // for MouseReport, MouseReport::ReportData
#include "testing/iostream.h"  // for string

namespace kaleidoscope {
namespace testing {

class ExpectedMouseReport {
 public:
  ExpectedMouseReport(uint32_t timestamp,
                      uint8_t buttons,
                      int8_t x,
                      int8_t y,
                      int8_t v,
                      int8_t h,
                      std::string message = "");

  uint8_t Buttons() const;
  int8_t XAxis() const;
  int8_t YAxis() const;
  int8_t VWheel() const;
  int8_t HWheel() const;

  uint32_t Timestamp() const;

  const std::string &Message() const;

 private:
  uint32_t timestamp_;
  MouseReport::ReportData report_data_;
  std::string failure_message_;
};

}  // namespace testing
}  // namespace kaleidoscope
