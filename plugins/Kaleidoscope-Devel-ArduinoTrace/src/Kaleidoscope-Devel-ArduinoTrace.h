/* -*- mode: c++ -*-
 * Kaleidoscope-Devel-ArduinoTrace -- an ArduinoTrace affordance for kaleidoscope debugging
 * Copyright (C) 2017  Keyboard.io, Inc
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

#ifndef ARDUINOTRACE_SERIAL
#ifdef KALEIDOSCOPE_VIRTUAL_BUILD
#define ARDUINOTRACE_SERIAL DebugStderr()
#endif
#endif

#include <HardwareSerial.h>  // for DebugStderrSerial, DebugStderr

#include "ArduinoTrace.h"  // for ARDUINOTRACE_INIT

ARDUINOTRACE_INIT(9600)
