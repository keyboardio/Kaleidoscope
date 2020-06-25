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
#include "papilio/aux/terminal_escape_sequences.h"
#include "kaleidoscope_simulator/vendors/keyboardio/model01.h"

#include <iostream>

KALEIDOSCOPE_SIMULATOR_INIT

namespace kaleidoscope {
namespace simulator {

void runSimulator(Simulator &simulator) {

  using namespace framework::terminal_escape_sequences;

  // Activate the rainbow wave LED effect
  //
  simulator.multiTapKey(2 /*num. taps*/,
                        0 /*row*/, 6/*col*/,
                        1 /* num. cycles after each tap */
                       );

  std::cout << clear_screen << std::flush;

  simulator.runRealtime(10000, // 50000 cycles
  [&]() {
    std::cout << cursor_to_upper_left << std::flush;
    renderKeyboard(simulator, keyboardio::model01::ascii_keyboard);
  }
                       );
}

} // namespace simulator
} // namespace kaleidoscope

#endif
