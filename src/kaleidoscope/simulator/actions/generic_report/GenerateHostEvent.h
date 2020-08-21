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
#ifdef KALEIDOSCOPE_VIRTUAL_HOST_EVENTS

#include "kaleidoscope/simulator/interface/actions/generic_report/ReportAction.h"
#include "kaleidoscope/simulator/interface/reports/Report_.h"

#include <cassert>

namespace kaleidoscope {
namespace simulator {
namespace actions {

/// @private
///
class HostEventAction {
 public:

  HostEventAction();

  ~HostEventAction();

 protected:

  void *display_ = nullptr;
};

/// @brief Generates an event that has the same effect as the report being
///        processed by the host.
///
template<typename _ReportType>
class GenerateHostEvent {
 public:

  SIMULATOR_ACTION_STD_CONSTRUCTOR(GenerateHostEvent)

 private:

  class Action : public interface::ReportAction<typename _ReportType::BaseReportType>,
      public HostEventAction {
public:

      virtual void describe(const char *add_indent = "") const override {
        this->getSimulator()->log() << add_indent << "Generating host event";
      }

      virtual void describeState(const char *add_indent = "") const override {
        this->describe(add_indent);
      }

      virtual bool evalInternal() override;

private:

      void cachePreviousReport() {
        previous_report_
          = std::static_pointer_cast<_ReportType>(this->getReport().clone());
      }

private:

      std::shared_ptr<_ReportType> previous_report_;
    };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(GenerateHostEvent<_ReportType>)
};

} // namespace actions
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_HOST_EVENTS
#endif // KALEIDOSCOPE_VIRTUAL_BUILD
