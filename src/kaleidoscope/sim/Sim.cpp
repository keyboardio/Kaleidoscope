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

#include "kaleidoscope/sim/Sim.h"
#include "Kaleidoscope.h"

namespace kaleidoscope {
namespace sim {

void Sim::RunCycle() {
  Kaleidoscope.loop();
}

void Sim::RunCycles(size_t n) {
  for (size_t i = 0; i < n; ++i) RunCycle();
}

void Sim::Press(uint8_t row, uint8_t, col) {
  Kaleidoscope.device().keyScanner().setKeystate(
      KeyAddr{row, col},
      Kaleidoscope::Device::Props::keyScanner::KeyState::Pressed);
}

}  // namespace sim
}  // namespace kaleidoscope
