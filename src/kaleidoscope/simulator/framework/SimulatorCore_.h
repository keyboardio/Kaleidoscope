/* -*- mode: c++ -*-
 * kaleidoscope::simulator::framework - A keyboard simulation framework
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

#include <string>
#include <stdint.h>

namespace kaleidoscope {
namespace simulator {
namespace framework {

/// @brief An interface to a specific keyboard.
///
class SimulatorCore_ {
 public:

  /// @brief Initializes the simulator core.
  ///
  virtual void init() = 0;

  /// @brief Retreives the dimension of the keyboard key matrix.
  /// @param rows The number of matrix rows.
  /// @param cols The number of matrix columns.
  ///
  virtual void getKeyMatrixDimensions(uint8_t &rows, uint8_t &cols) const = 0;

  /// @brief Presses a key of the simulated keyboard.
  /// @param row The row index of the key.
  /// @param col The column index of the key.
  ///
  virtual void pressKey(uint8_t row, uint8_t col) = 0;

  /// @brief Releases a key of the simulated keyboard.
  /// @param row The row index of the key.
  /// @param col The column index of the key.
  ///
  virtual void releaseKey(uint8_t row, uint8_t col) = 0;

  /// @brief Taps a key of the simulated keyboard.
  /// @param row The row index of the key.
  /// @param col The column index of the key.
  ///
  virtual void tapKey(uint8_t row, uint8_t col) = 0;

  /// @brief Checks if a key of the simulated keyboard is currently pressed.
  /// @param row The row index of the key.
  /// @param col The column index of the key.
  ///
  virtual bool isKeyPressed(uint8_t row, uint8_t col) const = 0;

  /// @brief Retrieves the numbers of LEDs
  ///
  /// @returns The number of LEDs
  //
  virtual uint8_t getNumLEDs() const = 0;

  /// @brief Retreives the current key LED color.
  ///
  /// @param[in] i The LED index.
  ///
  /// @param[out] red The red color portion.
  /// @param[out] green The green color portion.
  /// @param[out] blue The blue color portion.
  ///
  virtual void getCurrentKeyLEDColor(uint8_t key_offset,
                                     uint8_t &red,
                                     uint8_t &green,
                                     uint8_t &blue) const = 0;

  /// @brief The current label of the key.
  /// @details This is what describes briefly (few characters) what the
  ///            key currently does if pressed.
  ///
  /// @param[in] row The row index of the key.
  /// @param[in] col The column index of the key.
  /// @param[out] label_string The label string associated with the key.
  ///
  virtual void getCurrentKeyLabel(uint8_t row, uint8_t col,
                                  std::string &label_string) const = 0;

  /// @brief Sets the current time of the simulation.
  ///
  /// param time The current simulation time.
  ///
  virtual void setTime(uint32_t time) = 0;

  /// @brief Retrieves a short descriptive string that describes
  ///        a keycode.
  ///
  virtual const char *keycodeToName(uint8_t keycode) const = 0;

  /// @brief Run a single simulation loop cycle
  ///
  virtual void loop() = 0;
};

} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
