/* kailedoscope::sim - Simulator for Unit Testing Kaleidoscope
 * Copyright (C) 2020  epan <epaniagua@google.com>
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

#include "Kaleidoscope.h"
#include "testing/common/Simulator.h"

#include "testing/common/fix-macros.h"
#include <iostream>

namespace kaleidoscope {
namespace testing {

void Simulator::RunCycle() {
  std::cout << "epan 2.2.1" << std::endl;
  Kaleidoscope.loop();
  std::cout << "epan 2.2.2" << std::endl;
}

void Simulator::RunCycles(size_t n) {
  for (size_t i = 0; i < n; ++i) RunCycle();
}

void Simulator::Press(uint8_t row, uint8_t col) {
  Kaleidoscope.device().keyScanner().setKeystate(
      KeyAddr{row, col},
      kaleidoscope::Device::Props::KeyScanner::KeyState::Pressed);
}

void Simulator::Release(uint8_t row, uint8_t col) {
  Kaleidoscope.device().keyScanner().setKeystate(
      KeyAddr{row, col},
      kaleidoscope::Device::Props::KeyScanner::KeyState::NotPressed);
}

}  // namespace testing
}  // namespace kaleidoscope
