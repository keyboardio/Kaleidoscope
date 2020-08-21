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

#include "kaleidoscope/simulator/framework/actions/Action_.h"

namespace kaleidoscope {
namespace simulator {
namespace framework {
namespace actions {

/// @brief Asserts that the current cycle is the nth.
///
class AssertCycleIsNth {

 public:

  /// @brief Constructor
  /// @param cycle_id The id of the cycle to check against.
  ///
  AssertCycleIsNth(int cycle_id)
    : AssertCycleIsNth(DelegateConstruction{}, cycle_id)
  {}

 private:

  class Action : public Action_ {

   public:

    Action(int cycle_id) : cycle_id_(cycle_id) {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << "Is " << cycle_id_ << ". cycle";
    }

    virtual void describeState(const char *add_indent = "") const {
      this->getSimulator()->log() << add_indent << "Is " << this->getSimulator()->getCycleId() << ". cycle";
    }

   private:

    virtual bool evalInternal() override {
      return this->getSimulator()->getCycleId() == cycle_id_;
    }

   private:

    int cycle_id_ = -1;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY(AssertCycleIsNth)
};

} // namespace actions
} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
