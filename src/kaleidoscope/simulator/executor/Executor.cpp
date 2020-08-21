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

#include "kaleidoscope/simulator/executor/Executor.h"
#include "kaleidoscope/simulator/executor/SimulatorCore.h"
#include "kaleidoscope/simulator/executor/reports/KeyboardReport.h"
#include "kaleidoscope/simulator/executor/reports/BootKeyboardReport.h"
#include "kaleidoscope/simulator/executor/reports/MouseReport.h"
#include "kaleidoscope/simulator/executor/reports/AbsoluteMouseReport.h"

#include "Kaleidoscope.h"
#include "HIDReportObserver.h"

#include <iostream>

namespace kaleidoscope {
namespace simulator {
namespace executor {

Executor::Executor(std::ostream &out)
  :  framework::Simulator{out} {
  this->setCore(
  std::shared_ptr<SimulatorCore> {
    new SimulatorCore{}
  }
  );

  HIDReportObserver::resetHook(&Executor::processHIDReport);

  Kaleidoscope.device().keyScanner().setEnableReadMatrix(false);
}

Executor &Executor::getInstance() {
  static Executor sim{std::cout};
  return sim;
}

void Executor::processHIDReport(uint8_t id, const void* data,
                                int len, int result) {
  auto &simulator = Executor::getInstance();

  switch (id) {
  // TODO: React appropriately on the following
  //
  case HID_REPORTID_GAMEPAD:
  case HID_REPORTID_CONSUMERCONTROL:
  case HID_REPORTID_SYSTEMCONTROL:
    simulator.log() << "***Ignoring hid report with id = " << id;
    break;
  case HID_REPORTID_KEYBOARD: {
    simulator.processReport(BootKeyboardReport{data});
  }
  break;
  case HID_REPORTID_MOUSE_ABSOLUTE: {
    simulator.processReport(AbsoluteMouseReport{data});
  }
  break;
  case HID_REPORTID_MOUSE: {
    simulator.processReport(MouseReport{data});
  }
  break;
  case HID_REPORTID_NKRO_KEYBOARD: {
    simulator.processReport(KeyboardReport{data});
  }
  break;
  default:
    simulator.error() << "Encountered unknown HID report with id = " << id;
  }
}

} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
