/* -*- mode: c++ -*-
 * Kaleidoscope-CycleTimeReport -- Scan cycle time reporting
 * Copyright (C) 2017, 2018  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-CycleTimeReport.h>

namespace kaleidoscope {
uint32_t CycleTimeReport::next_report_time_;
uint32_t CycleTimeReport::loop_start_time_;
uint32_t CycleTimeReport::average_loop_time;

EventHandlerResult CycleTimeReport::onSetup() {
  next_report_time_ = millis() + 1000;
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

  if (millis() >= next_report_time_) {
    cycleTimeReport();

    average_loop_time = 0;
    next_report_time_ = millis() + 1000;
  }

  return EventHandlerResult::OK;
}

// Deprecated V1 APIs
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void CycleTimeReport::begin() {
  Kaleidoscope.useLoopHook(legacyLoopHook);
}

void CycleTimeReport::legacyLoopHook(bool is_post_clear) {
  if (is_post_clear)
    ::CycleTimeReport.afterEachCycle();
  else
    ::CycleTimeReport.beforeEachCycle();
}
#endif

}

__attribute__((weak)) void cycleTimeReport(void) {
  Serial.print(F("# average loop time: "));
  Serial.println(CycleTimeReport.average_loop_time);
}

kaleidoscope::CycleTimeReport CycleTimeReport;
