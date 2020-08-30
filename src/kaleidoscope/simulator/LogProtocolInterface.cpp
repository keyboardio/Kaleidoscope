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

#include "kaleidoscope/simulator/LogProtocolInterface.h"
#include "kaleidoscope/simulator/reports/KeyboardReport.h"
#include "kaleidoscope/simulator/reports/BootKeyboardReport.h"
#include "kaleidoscope/simulator/reports/MouseReport.h"
#include "kaleidoscope/simulator/reports/AbsoluteMouseReport.h"
#include "kaleidoscope/simulator/LogProtocol.h"
#include "kaleidoscope/simulator/log_protocol/Consumer_.h"
#include "kaleidoscope/simulator/interface/actions/generic_report/AssertReportEquals.h"
#include "kaleidoscope/simulator/interface/Simulator_.h"
#include "HID-Settings.h"

namespace kaleidoscope {
namespace simulator {

/// @private
///
class SimulatorConsumerAdaptor : public log_protocol::Consumer_ {
 public:

  SimulatorConsumerAdaptor(interface::Simulator_ &simulator)
    :  simulator_(simulator)
  {}

  virtual void onFirmwareId(const char *firmware_id) override {
    simulator_.log() << "Aglais: firmware_id " << firmware_id;
    // TODO: Use this method to verify that the firmware that was used
    //       to generate the Aglais-script that is currently
    //       parsed matches the firmware running in the simulator.
  }

  virtual void onStartCycle(uint32_t cycle_id, uint32_t cycle_start_time) override {
    //simulator_.log() << "Aglais: start_cycle " << cycle_id << ' ' << cycle_start_time;
    simulator_.setTime(cycle_start_time);
  }
  virtual void onEndCycle(uint32_t cycle_id, uint32_t cycle_end_time) override {
    //simulator_.log() << "Aglais: end_cycle " << cycle_id << ' ' << cycle_end_time;

    simulator_.cycle(true /*suppress cycle log info*/);

    if (!simulator_.reportActionsQueue().empty()) {
      simulator_.error() << "Report actions are left in queue";
    }

    simulator_.setTime(cycle_end_time);
  }
  virtual void onKeyPressed(uint8_t row, uint8_t col) override {
    simulator_.log() << "Aglais: action key_pressed " << (int)row << ' ' << (int)col;
    simulator_.pressKey(row, col);
  }
  virtual void onKeyReleased(uint8_t row, uint8_t col) override {
    simulator_.log() << "Aglais: action key_released " << (int)row << ' ' << (int)col;
    simulator_.releaseKey(row, col);
  }
  virtual void onHIDReport(uint8_t id, int length, const uint8_t *data) override {
    {
      auto log = simulator_.log();

      log << "Aglais: reaction hid_report " << (int)id << ' ' << (int)length << ' ';
      for (int i = 0; i < length; ++i) {
        log << (int)data[i] << ' ';
      }
    }

    switch (id) {
    // TODO: React appropriately on the following
    //
    case HID_REPORTID_GAMEPAD:
    case HID_REPORTID_CONSUMERCONTROL:
    case HID_REPORTID_SYSTEMCONTROL:
      simulator_.log() << "***Ignoring hid report with id = " << id;
      break;
    case HID_REPORTID_KEYBOARD: {
      assert(length == sizeof(BootKeyboardReport::ReportDataType));
      simulator_.reportActionsQueue().queue(
        interface::actions::AssertReportEquals<BootKeyboardReport> {data}
      );
    }
    break;
    case HID_REPORTID_MOUSE_ABSOLUTE: {
      assert(length == sizeof(AbsoluteMouseReport::ReportDataType));
      simulator_.reportActionsQueue().queue(
        interface::actions::AssertReportEquals<AbsoluteMouseReport> {data}
      );
    }
    break;
    case HID_REPORTID_MOUSE: {
      assert(length == sizeof(MouseReport::ReportDataType));
      simulator_.reportActionsQueue().queue(
        interface::actions::AssertReportEquals<MouseReport> {data}
      );
    }
    break;
    case HID_REPORTID_NKRO_KEYBOARD: {
      assert(length == sizeof(KeyboardReport::ReportDataType));
      simulator_.reportActionsQueue().queue(
        interface::actions::AssertReportEquals<KeyboardReport> {data}
      );
    }
    break;
    default:
      simulator_.error() << "Aglais encountered unknown HID report with id = " << id;
    }
  }
  virtual void onSetTime(uint32_t time) override {
    simulator_.log() << "Aglais: set_time " << time;
    simulator_.setTime(time);
  }
  virtual void onCycle(uint32_t cycle_id, uint32_t cycle_start_time, uint32_t cycle_end_time) {
    this->onStartCycle(cycle_id, cycle_start_time);
    this->onEndCycle(cycle_id, cycle_end_time);
  }
  virtual void onCycles(uint32_t start_cycle_id, uint32_t start_time_id,
                        const std::vector<uint32_t> &cycle_durations) override {
    auto cycle_start_time = start_time_id;
    auto cycle_id = start_cycle_id;
    for (const auto duration : cycle_durations) {
      auto cycle_end_time = cycle_start_time + duration;
      this->onCycle(cycle_id, cycle_start_time, cycle_end_time);
      ++cycle_id;
      cycle_start_time = cycle_end_time;
    }
  }

 private:

  interface::Simulator_ &simulator_;

};

void processLogDocument(const char *code, interface::Simulator_ &simulator) {
  auto rwqa_state = simulator.getErrorIfReportWithoutQueuedActions();

  log_protocol::LogProtocol a;
  //a.setDebug(true);

  SimulatorConsumerAdaptor sca(simulator);
  a.parse(code, sca);

  simulator.setErrorIfReportWithoutQueuedActions(rwqa_state);
}

} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
