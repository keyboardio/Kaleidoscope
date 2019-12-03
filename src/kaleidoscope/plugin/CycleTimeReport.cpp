/* -*- mode: c++ -*-
 * Kaleidoscope-CycleTimeReport -- Scan cycle time reporting
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-CycleTimeReport.h>
#include <Kaleidoscope-FocusSerial.h>

namespace kaleidoscope {
namespace plugin {
uint16_t CycleTimeReport::last_report_time_;
uint32_t CycleTimeReport::loop_start_time_;
uint32_t CycleTimeReport::average_loop_time;

EventHandlerResult CycleTimeReport::onSetup() {
  last_report_time_ = Runtime.millisAtCycleStart();
  return EventHandlerResult::OK;
}

EventHandlerResult CycleTimeReport::beforeEachCycle() {
  loop_start_time_ = micros();
  return EventHandlerResult::OK;
}

EventHandlerResult CycleTimeReport::afterEachCycle() {
  uint32_t loop_time = micros() - loop_start_time_;

  if (average_loop_time)
    average_loop_time = (average_loop_time + loop_time) / 2;
  else
    average_loop_time = loop_time;

  if (Runtime.hasTimeExpired(last_report_time_, uint16_t(1000))) {
    cycleTimeReport();

    average_loop_time = 0;
    last_report_time_ = Runtime.millisAtCycleStart();
  }

  return EventHandlerResult::OK;
}

}
}

__attribute__((weak)) void cycleTimeReport(void) {
  Focus.send(Focus.COMMENT, F("average loop time:"), CycleTimeReport.average_loop_time,
             Focus.NEWLINE);
}

kaleidoscope::plugin::CycleTimeReport CycleTimeReport;
