/* -*- mode: c++ -*-
 * Kaleidoscope-CycleTimeReport -- Scan cycle time reporting
 * Copyright (C) 2017  Gergely Nagy
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

void CycleTimeReport::begin (void) {
  Kaleidoscope.useLoopHook(loopHook);
  next_report_time_ = millis() + 1000;
}

void CycleTimeReport::loopHook(bool post_clear) {
  if (!post_clear)
    return;

  if (loop_start_time_) {
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
    loop_start_time_ = 0;
  } else {
    loop_start_time_ = micros();
  }
}
}

__attribute__((weak)) void cycleTimeReport(void) {
  Serial.print(F("# average loop time: "));
  Serial.println(CycleTimeReport.average_loop_time);
}

kaleidoscope::CycleTimeReport CycleTimeReport;
