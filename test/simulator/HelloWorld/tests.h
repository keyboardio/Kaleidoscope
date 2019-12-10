/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

  // On X Window systems, Kaleidoscope-Simulator supports the generation of
  // host events. This allows to let the keyboard simulator behave like
  // a real keyboard. All virtual key actions generate keyboard reports
  // that are passed directly to the OS.
  //
  // Uncomment the following to generate host events.
  //
//    simulator.permanentBootKeyboardReportActions().add(GenerateHostEvent<BootKeyboardReport>{});
//    simulator.permanentKeyboardReportActions().add(GenerateHostEvent<KeyboardReport>{});
//    simulator.permanentMouseReportActions().add(GenerateHostEvent<MouseReport>{});
//    simulator.permanentAbsoluteMouseReportActions().add(GenerateHostEvent<AbsoluteMouseReport>{});

  processAglaisDocument(aglais_test_recording, simulator);
}

const char aglais_test_recording[] =
#include "IO_protocol.agl"
  ;

} // namespace simulator
} // namespace kaleidoscope

#endif
