/* -*- mode: c++ -*-
 * kaleidoscope::simulator - Testing API for Kaleidoscope
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/simulator/framework/aux/WallTimer.h"

namespace kaleidoscope {
namespace simulator {
namespace framework {
namespace aux {

void WallTimer::start() {
  start_time_ = std::chrono::high_resolution_clock::now();
}

double WallTimer::elapsed() {
  static constexpr double inv_clocks_per_sec = 1.0 / CLOCKS_PER_SEC;
  auto cur_time = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(cur_time - start_time_).count();
}

} // namespace aux
} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
