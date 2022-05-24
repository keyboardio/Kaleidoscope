/* -*- mode: c++ -*-
 * Copyright (C) 2020  Keyboard.io, Inc
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

#include "testing/ExpectedKeyboardReport.h"

#include <cstdint>  // for uint8_t, uint32_t
#include <set>      // for set

namespace kaleidoscope {
namespace testing {

ExpectedKeyboardReport::ExpectedKeyboardReport(uint32_t timestamp,
                                               const std::set<uint8_t> &keycodes,
                                               std::string message) {
  timestamp_       = timestamp;
  keycodes_        = std::set<uint8_t>(keycodes);
  failure_message_ = message;
}

const std::set<uint8_t> &ExpectedKeyboardReport::Keycodes() const {
  return keycodes_;
}

uint32_t ExpectedKeyboardReport::Timestamp() const {
  return timestamp_;
}

const std::string &ExpectedKeyboardReport::Message() const {
  return failure_message_;
}

}  // namespace testing
}  // namespace kaleidoscope
