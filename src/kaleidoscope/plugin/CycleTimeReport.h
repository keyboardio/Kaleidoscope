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

#pragma once

#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace plugin {
class CycleTimeReport : public kaleidoscope::Plugin {
 public:
  CycleTimeReport() {}

  EventHandlerResult onSetup();
  EventHandlerResult beforeEachCycle();
  EventHandlerResult afterEachCycle();

  static uint32_t average_loop_time;

 private:
  static uint16_t last_report_time_;
  static uint32_t loop_start_time_;
};
}
}

void cycleTimeReport(void);

extern kaleidoscope::plugin::CycleTimeReport CycleTimeReport;
