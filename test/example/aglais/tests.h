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
#include "kaleidoscope_simulator/AglaisInterface.h"

#include <iostream>
#include <sstream>

KALEIDOSCOPE_SIMULATOR_INIT

namespace kaleidoscope {
namespace simulator {

extern const char aglais_test_recording[];

void runSimulator(Simulator &simulator) {

  using namespace actions;

  //simulator.setQuiet();

  auto test = simulator.newTest("Aglais test");

//    simulator.permanentBootKeyboardReportActions().add(GenerateHostEvent<BootKeyboardReport>{});
//    simulator.permanentKeyboardReportActions().add(GenerateHostEvent<KeyboardReport>{});
//    simulator.permanentMouseReportActions().add(GenerateHostEvent<MouseReport>{});
//    simulator.permanentAbsoluteMouseReportActions().add(GenerateHostEvent<AbsoluteMouseReport>{});

  processAglaisDocument(aglais_test_recording, simulator);
}

const char aglais_test_recording[] =
#include "IO_protocoll.agl"
  ;

} // namespace simulator
} // namespace kaleidoscope

#endif
