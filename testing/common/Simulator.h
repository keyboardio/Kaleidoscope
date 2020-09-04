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

#pragma once

#include <cstddef>
#include <cstdint>

namespace kaleidoscope {
namespace testing {

class Simulator {
 public:
  void RunCycle();
  void RunCycles(size_t n);

  void Press(uint8_t row, uint8_t col);
  void Release(uint8_t row, uint8_t col);
};

}  // namespace testing
}  // namespace kaleidoscope
