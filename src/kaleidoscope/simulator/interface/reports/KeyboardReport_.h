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

#include "kaleidoscope/simulator/interface/reports/Report_.h"

// Undefine some macros defined by Arduino
//
#undef min
#undef max

#include <vector>
#include <stdint.h>
#include <ostream>

namespace kaleidoscope {
namespace simulator {
namespace interface {

    class Simulator_;

/// @brief An interface hat facilitates analyzing keyboard reports.
///
    class KeyboardReport_ : public Report_ {

     public:

      static constexpr uint8_t type_ = KeyboardReportTypeId;

      typedef KeyboardReport_ BaseReportType;

      /// @brief Checks if a keycode is active in the keyboard report.
      /// @param keycode The keycode to check for.
      /// @returns [bool] True if the given keycode is active.
      ///
      virtual bool isKeycodeActive(uint8_t keycode) const = 0;

      /// @brief Retreives a list of all keycodes that are active in the
      ///        keyboard report.
      /// @returns A vector of keycodes.
      ///
      virtual std::vector<uint8_t> getActiveKeycodes() const = 0;

      /// @brief Checks if a modifier keycode is active in the keyboard report.
      /// @param keycode The modifier keycode to check for.
      /// @returns [bool] True if the given modifier keycode is active.
      ///
      virtual bool isModifierKeycodeActive(uint8_t modifier) const = 0;

      /// @brief Checks if any modifier keycode is active.
      /// @returns [bool] True if any modifier keycode is active, false otherwise.
      ///
      virtual bool isAssertAnyModifierActive() const = 0;

      /// @brief Checks if any key keycode is active.
      /// @returns [bool] True if any key keycode is active, false otherwise.
      ///
      virtual bool isAnyKeyActive() const = 0;

      /// @brief Retreives a list of active modifier keycodes.
      /// @returns A list of active modifier keycodes.
      ///
      virtual std::vector<uint8_t> getActiveModifiers() const = 0;

      static const char *typeString() {
        return "keyboard";
      }
      virtual const char *getTypeString() const override {
        return typeString();
      }
    };

  } // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
