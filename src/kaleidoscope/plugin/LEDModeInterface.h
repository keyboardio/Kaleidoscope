/*
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

#include <Arduino.h>  // for PROGMEM

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/event_handlers.h"        // for LedModeCallback

namespace kaleidoscope {
namespace plugin {

class LEDModeInterface {
 public:
  LEDModeInterface()
    : led_mode_name_{0} {}
  explicit LEDModeInterface(const char *led_mode_name)
    : led_mode_name_{led_mode_name} {}
  void activate();

  EventHandlerResult onLedEffectQuery(LedModeCallback callback) {
    if (led_mode_name_ == 0) {
      // If no name was defined, return a default string
      callback("[unnamed led mode]");
    } else {
      callback(led_mode_name_);
    }
    return EventHandlerResult::OK;
  }

  // This auxiliary class helps to generate a verbose error message
  // in case that there is no TransientLEDMode typedef or nested
  // class present in a derived class of LEDModeInterface.
  //
  struct _____LEDModeInterface_derived_class_is_missing_a_TransientLEDMode_typedef_or_nested_class_____ {};
  typedef _____LEDModeInterface_derived_class_is_missing_a_TransientLEDMode_typedef_or_nested_class_____ NoLEDMode;

  // By redefining TransientLEDMode, derived plugins export
  // a LED mode that becomes part of the set of LED modes whose
  // lifetime is handled dynamically.
  //
  typedef NoLEDMode DynamicLEDMode;

 protected:
  const char *led_mode_name_ PROGMEM;
};

}  // namespace plugin
}  // namespace kaleidoscope
