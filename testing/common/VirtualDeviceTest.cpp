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

#include "testing/common/VirtualDeviceTest.h"

namespace kaleidoscope {
namespace testing {

void VirtualDeviceTest::RunCycle() {
  std::cout << "epan 2.1" << std::endl;
  state_.Clear();
  std::cout << "epan 2.2" << std::endl;
  sim_.RunCycle();
  std::cout << "epan 2.3" << std::endl;
}

void VirtualDeviceTest::RunCycles(size_t n) {
  if (n == 0) return;
  state_.Clear();
  sim_.RunCycles(n);
}

const State& VirtualDeviceTest::Result() const {
  return state_;
}

}  // namespace testing
}  // namespace kaleidoscope
