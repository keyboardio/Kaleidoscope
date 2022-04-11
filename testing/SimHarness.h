/* kailedoscope::sim - Simulator for Unit Testing Kaleidoscope
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

#pragma once

#include <cstddef>  // for size_t
#include <cstdint>  // for uint8_t

#include "kaleidoscope/KeyAddr.h"  // for KeyAddr
#include "testing/gtest.h"         // IWYU pragma: keep

namespace kaleidoscope {
namespace testing {

class SimHarness {
 public:
  void RunCycle();
  void RunCycles(size_t n);
  void RunForMillis(size_t t);
  void Press(KeyAddr key_addr);
  void Release(KeyAddr key_addr);
  void Press(uint8_t row, uint8_t col);
  void Release(uint8_t row, uint8_t col);
  void SetCycleTime(uint8_t millis);
  uint8_t CycleTime() const;

 private:
  uint8_t millis_per_cycle_ = 1;
};

}  // namespace testing
}  // namespace kaleidoscope
