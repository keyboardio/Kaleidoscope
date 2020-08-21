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

#include "kaleidoscope/simulator/framework/reports/Report_.h"

// Undefine some macros defined by Arduino
//
#undef min
#undef max

#include <stdint.h>
#include <ostream>

namespace kaleidoscope {
namespace simulator {
namespace framework {

class Simulator;

/// @brief An interface hat facilitates analyzing mouse reports.
///
class MouseReport_ : public Report_ {

 public:

  static constexpr uint8_t type_ = MouseReportTypeId;

  typedef MouseReport_ BaseReportType;

  /// @brief Checks if a set of buttons is pressed.
  /// @param button_state The state of the mouse buttons to check.
  /// @returns True if the button state matches the given one.
  ///
  virtual bool areButtonsPressed(uint8_t button_state) const = 0;

  /// @brief Queries if the left button is pressed.
  /// @returns True if the left button is pressed.
  ///
  virtual bool isLeftButtonPressed() const = 0;

  /// @brief Queries if the middle button is pressed.
  /// @returns True if the middle button is pressed.
  ///
  virtual bool isMiddleButtonPressed() const = 0;

  /// @brief Queries if the right button is pressed.
  /// @returns True if the right button is pressed.
  ///
  virtual bool isRightButtonPressed() const = 0;

  /// @brief Queries the x-movement stored in the report.
  /// @returns The x-movement.
  ///
  virtual int8_t getXMovement() const = 0;

  /// @brief Queries the y-movement stored in the report.
  /// @returns The y-movement.
  ///
  virtual int8_t getYMovement() const = 0;

  /// @brief Queries the verical wheel movement.
  /// @returns The vertical wheel movement.
  ///
  virtual int8_t getVerticalWheel() const = 0;

  /// @brief Queries the horizontal wheel movement.
  /// @returns The horizontal wheel movement.
  ///
  virtual int8_t getHorizontalWheel() const = 0;

  static const char *typeString() {
    return "mouse";
  }
  virtual const char *getTypeString() const override {
    return typeString();
  };
};

} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
