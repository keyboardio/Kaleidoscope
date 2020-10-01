/* Kaleidoscope-Model01-TestMode - A factory test mode for the Model 01.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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
#error "Model01 testmode not available for virtual builds"
#else

#ifdef ARDUINO_AVR_MODEL01

#include <Arduino.h>
#include "kaleidoscope/Runtime.h"
#include "kaleidoscope_internal/deprecations.h"

#define _DEPRECATED_MESSAGE_MODEL01_TESTMODE                                      \
  "The Model01-specific TestMode plugin has been deprecated in favour of\n" __NL__ \
  "the more generic HardwareTestMode. Please migrate to the new one."

namespace kaleidoscope {
namespace plugin {

class TestMode : public kaleidoscope::Plugin {
 public:
  DEPRECATED(MODEL01_TESTMODE) EventHandlerResult beforeReportingState() {}
};
}
}

extern kaleidoscope::plugin::TestMode TestMode;

#endif

#endif
