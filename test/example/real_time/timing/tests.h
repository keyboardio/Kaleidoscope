/* -*- mode: c++ -*-
 * Kaleidoscope-Simulator -- A C++ testing API for the Kaleidoscope keyboard
 *                         firmware.
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
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

#include "Kaleidoscope-Simulator.h"

#include <iostream>
#include <ctime>

KALEIDOSCOPE_SIMULATOR_INIT

namespace kaleidoscope {
namespace simulator {

void runSimulator(Simulator &simulator) {

  // Loop cycle timing
  auto begin = std::clock();

  static constexpr int n_cycles = 10000;
  static constexpr double inv_clocks_per_sec = 1.0 / CLOCKS_PER_SEC;

  simulator.cycles(n_cycles);

  auto end = std::clock();
  double elapsed_secs = double(end - begin) * inv_clocks_per_sec;
  double elapsed_secs_per_cycle = elapsed_secs / n_cycles;

  simulator.log() << "elapsed [s]: " << elapsed_secs;
  simulator.log() << "cycle duration: " << elapsed_secs_per_cycle;
}

} // namespace simulator
} // namespace kaleidoscope

#endif
