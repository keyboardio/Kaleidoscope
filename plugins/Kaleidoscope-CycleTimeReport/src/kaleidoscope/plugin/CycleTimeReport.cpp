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

#include "kaleidoscope/plugin/CycleTimeReport.h"

#include <Arduino.h>                   // for micros, F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for uint16_t, uint32_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK

namespace kaleidoscope {
namespace plugin {

EventHandlerResult CycleTimeReport::beforeEachCycle() {
  // A counter storing the number of cycles since the last mean cycle time
  // report was sent:
  static uint16_t elapsed_cycles = 0;

  // We only compute the mean cycle time once per report interval.
  if (Runtime.hasTimeExpired(last_report_millis_, report_interval_)) {
    uint32_t elapsed_time    = micros() - last_report_micros_;
    uint32_t mean_cycle_time = elapsed_time / elapsed_cycles;

    report(mean_cycle_time);

    // Reset the cycle counter and timestamps.
    elapsed_cycles = 0;
    last_report_millis_ += report_interval_;
    last_report_micros_ = micros();
  }

  // Increment the cycle counter unconditionally.
  ++elapsed_cycles;

  return EventHandlerResult::OK;
}

__attribute__((weak)) void CycleTimeReport::report(uint16_t mean_cycle_time) {
  Focus.send(Focus.COMMENT,
             F("mean cycle time:"),
             mean_cycle_time,
             F("µs"),
             Focus.NEWLINE);
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::CycleTimeReport CycleTimeReport;
