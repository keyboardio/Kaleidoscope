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

#include "kaleidoscope/simulator/framework/actions/generic_report/ReportAction.h"

#include <functional>

namespace kaleidoscope {
namespace simulator {
namespace framework {
namespace actions {

/// @brief Executes a lambda function of type bool(const _ReportType&).
/// @details The lambda must return true to signal that the action passed
///        and false otherwise.
///
template<typename _ReportType>
class CustomReportAction {

 public:

  /// @brief Constructor.
  /// @param func The function to evaluate as a condition for
  ///        the action to pass.
  ///
  CustomReportAction(const std::function<bool(const _ReportType&)> &func)
    : CustomReportAction(DelegateConstruction{}, func)
  {}

 private:

  class Action : public ReportAction<_ReportType> {

   public:

    using ReportAction<_ReportType>::ActionBaseType;

    Action(const std::function<bool(const _ReportType&)> &func)
      : func_(func)
    {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << "Custom "
                                  << _ReportType::typeString() << " report action";
    }

    virtual void describeState(const char *add_indent = "") const {
      this->getSimulator()->log() << add_indent << "Custom "
                                  << _ReportType::typeString() << " report action failed";
    }

    virtual bool evalInternal() override {
      return func_(this->getReport());
    }

   private:

    std::function<bool(const _ReportType&)> func_;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(CustomReportAction<_ReportType>)
};

} // namespace actions
} // namespace framework
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
