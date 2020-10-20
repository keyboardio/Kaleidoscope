/* kailedoscope::sim - Harness for Unit Testing Kaleidoscope
 * Copyright (C) 2020  Eric Paniagua <epaniagua@google.com>
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

#include "testing/SimHarness.h"

#include "testing/fix-macros.h"
#include <iostream>

namespace kaleidoscope {
namespace testing {

void SimHarness::RunCycle() {
  Kaleidoscope.loop();
}

void SimHarness::RunCycles(size_t n) {
  for (size_t i = 0; i < n; ++i) RunCycle();
}

void SimHarness::Press(KeyAddr key_addr) {
  Kaleidoscope.device().keyScanner().setKeystate(
    key_addr,
    kaleidoscope::Device::Props::KeyScanner::KeyState::Pressed);
}

void SimHarness::Release(KeyAddr key_addr) {
  Kaleidoscope.device().keyScanner().setKeystate(
    key_addr,
    kaleidoscope::Device::Props::KeyScanner::KeyState::NotPressed);
}

void SimHarness::Press(uint8_t row, uint8_t col) {
  Press(KeyAddr{row, col});
}

void SimHarness::Release(uint8_t row, uint8_t col) {
  Release(KeyAddr{row, col});
}

}  // namespace testing
}  // namespace kaleidoscope
