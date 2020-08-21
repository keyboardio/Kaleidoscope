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

#include "kaleidoscope/simulator/interface/actions/Action_.h"

#include <functional>

namespace kaleidoscope {
namespace simulator {
namespace interface {
namespace actions {

/// @brief Executes a lambda function of type bool(const Simulator &).
/// @details The lambda must return true to signal that the action passed
///        and false otherwise.
///
class CustomAction {

 public:

  /// @brief Constructor.
  /// @param func The function object to evaluate.
  ///
  CustomAction(const std::function<bool()> &func)
    : CustomAction(DelegateConstruction{}, func)
  {}

 private:

  class Action : public Action_ {

   public:

    Action(const std::function<bool()> &func)
      : func_(func)
    {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << "Custom keyboard report action";
    }

    virtual void describeState(const char *add_indent = "") const {
      this->getSimulator()->log() << add_indent << "Custom keyboard report action failed";
    }

    virtual bool evalInternal() override {
      return func_();
    }

   private:

    std::function<bool()> func_;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY(CustomAction)
};

} // namespace actions
} // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
