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

#include <stdint.h>  // for uint16_t, uint32_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin
// -----------------------------------------------------------------------------
// Deprecation warning messages
#include "kaleidoscope_internal/deprecations.h"  // for DEPRECATED

#define _DEPRECATED_MESSAGE_CYCLETIMEREPORT_AVG_TIME                      \
  "The `CycleTimeReport.average_loop_time` variable is deprecated. See\n" \
  "the current documentation for CycleTimeReport for details.\n"          \
  "This variable will be removed after 2022-09-01."
// -----------------------------------------------------------------------------

namespace kaleidoscope {
namespace plugin {
class CycleTimeReport : public kaleidoscope::Plugin {
 public:
  EventHandlerResult beforeEachCycle();

#ifndef NDEPRECATED
  DEPRECATED(CYCLETIMEREPORT_AVG_TIME)
  static uint32_t average_loop_time;
#endif

  /// Set the length of time between reports (in milliseconds)
  void setReportInterval(uint16_t interval) {
    report_interval_ = interval;
  }

  /// Report the given mean cycle time in microseconds
  void report(uint16_t mean_cycle_time);

 private:
  // Interval between reports, in milliseconds
  uint16_t report_interval_ = 1000;

  // Timestamps recording when the last report was sent
  uint16_t last_report_millis_ = 0;
  uint32_t last_report_micros_ = 0;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::CycleTimeReport CycleTimeReport;
