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

#pragma once

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/simulator/interface/Simulator.h"
#include "kaleidoscope/simulator/interface/Visualization.h"
#include "kaleidoscope/simulator/interface/LED_Checks.h"

#include "kaleidoscope/simulator/interface/reports/BootKeyboardReport_.h"
#include "kaleidoscope/simulator/interface/reports/KeyboardReport_.h"
#include "kaleidoscope/simulator/interface/reports/MouseReport_.h"
#include "kaleidoscope/simulator/interface/reports/AbsoluteMouseReport_.h"

#include "kaleidoscope/simulator/interface/actions/Grouped.h"
#include "kaleidoscope/simulator/interface/actions/CustomAction.h"
#include "kaleidoscope/simulator/interface/actions/AssertNumOverallReportsEquals.h"
#include "kaleidoscope/simulator/interface/actions/Action_.h"
#include "kaleidoscope/simulator/interface/actions/AssertCycleIsNth.h"
#include "kaleidoscope/simulator/interface/actions/AssertElapsedTimeGreater.h"

#include "kaleidoscope/simulator/interface/actions/generic_report/AssertReportEmpty.h"
#include "kaleidoscope/simulator/interface/actions/generic_report/AssertReportEquals.h"
#include "kaleidoscope/simulator/interface/actions/generic_report/DumpReport.h"
#include "kaleidoscope/simulator/interface/actions/generic_report/AssertReportIsNthInCycle.h"
#include "kaleidoscope/simulator/interface/actions/generic_report/CustomReportAction.h"
#include "kaleidoscope/simulator/interface/actions/generic_report/AssertCycleGeneratesNReports.h"

#include "kaleidoscope/simulator/interface/actions/keyboard_report/AssertModifiersActive.h"
#include "kaleidoscope/simulator/interface/actions/keyboard_report/AssertAnyKeycodeActive.h"
#include "kaleidoscope/simulator/interface/actions/keyboard_report/AssertKeycodesActive.h"
#include "kaleidoscope/simulator/interface/actions/keyboard_report/AssertAnyModifierActive.h"

#endif
