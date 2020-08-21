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

#include "kaleidoscope/simulator/interface/SimulatorCore_.h"

namespace kaleidoscope {
namespace simulator {

/// @brief A Kaleidoscope specific simulator core class.
///
class SimulatorCore : public interface::SimulatorCore_ {
 public:

  virtual void init() override;

  virtual void getKeyMatrixDimensions(uint8_t &rows, uint8_t &cols) const override;

  virtual void pressKey(uint8_t row, uint8_t col) override;

  virtual void releaseKey(uint8_t row, uint8_t col) override;

  virtual void tapKey(uint8_t row, uint8_t col) override;

  virtual bool isKeyPressed(uint8_t row, uint8_t col) const override;

  virtual uint8_t getNumLEDs() const override;

  virtual void getCurrentKeyLEDColor(uint8_t key_offset,
                                     uint8_t &red, uint8_t &green, uint8_t &blue) const override;

  virtual void getCurrentKeyLabel(uint8_t row, uint8_t col,
                                  std::string &label_string) const override;

  virtual void setTime(uint32_t time) override;

  virtual const char *keycodeToName(uint8_t keycode) const override;

  virtual void loop() override;
};

} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
