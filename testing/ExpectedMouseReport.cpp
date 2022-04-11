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

#include "testing/ExpectedMouseReport.h"

#include "MultiReport/Mouse.h"    // for (anonymous union)::(anonymous)
#include "testing/MouseReport.h"  // for MouseReport::ReportData

namespace kaleidoscope {
namespace testing {

ExpectedMouseReport::ExpectedMouseReport(uint32_t timestamp,
                                         uint8_t buttons,
                                         int8_t x,
                                         int8_t y,
                                         int8_t v,
                                         int8_t h,
                                         std::string message) {
  timestamp_           = timestamp;
  report_data_.buttons = buttons;
  report_data_.xAxis   = x;
  report_data_.yAxis   = y;
  report_data_.vWheel  = v;
  report_data_.hWheel  = h;
  failure_message_     = message;
}

uint32_t ExpectedMouseReport::Timestamp() const {
  return timestamp_;
}

const std::string &ExpectedMouseReport::Message() const {
  return failure_message_;
}

uint8_t ExpectedMouseReport::Buttons() const {
  return report_data_.buttons;
}

int8_t ExpectedMouseReport::XAxis() const {
  return report_data_.xAxis;
}

int8_t ExpectedMouseReport::YAxis() const {
  return report_data_.yAxis;
}

int8_t ExpectedMouseReport::VWheel() const {
  return report_data_.vWheel;
}

int8_t ExpectedMouseReport::HWheel() const {
  return report_data_.hWheel;
}

}  // namespace testing
}  // namespace kaleidoscope
