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

#include <cstddef>
#include <cstdint>
#include <vector>

#include "testing/HIDState.h"

// Out of order due to macro conflicts.
#include "testing/fix-macros.h"
#include <memory>

namespace kaleidoscope {
namespace testing {

class State {
 public:
  static std::unique_ptr<State> Snapshot();

  const HIDState* HIDReports() const;

 private:
  std::unique_ptr<HIDState> hid_state_;
};

}  // namespace testing
}  // namespace kaleidoscope
