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

#include "kaleidoscope/simulator/interface/Simulator.h"
#include "kaleidoscope/simulator/interface/actions/Action_.h"
#include "kaleidoscope/simulator/interface/actions/generic_report/ReportAction.h"

#include <vector>

/// @file
/// @brief This files contains classes and functions to
///        group actions.

namespace kaleidoscope {
namespace simulator {
namespace interface {
namespace actions {

/// @private
///
template<typename _ActionType>
class GroupedAction_ : public _ActionType {

 public:

  template<typename..._Actions>
  GroupedAction_(_Actions...actions)
    :  actions_{std::forward<_Actions>(actions)...}
  {}

  GroupedAction_(const std::vector<std::shared_ptr<_ActionType>> &actions)
    :  actions_{actions}
  {}

  virtual void report(const char *add_indent = "") const override {
    this->getSimulator()->log() << add_indent << "Action group:";
    std::string indent = std::string(add_indent) + "   ";
    for (const auto &action : actions_) {
      action->report(indent.c_str());
    }
  }

  virtual void setSimulator(const Simulator *simulator) override {
    this->Action_::setSimulator(simulator);

    for (auto &action : actions_) {
      action->setSimulator(simulator);
    }
  }

  virtual void describe(const char *add_indent = "") const override {
    this->getSimulator()->log() << "A group of actions";
  }

  virtual void describeState(const char *add_indent = "") const override {
    if (valid_) {
      this->getSimulator()->log() << add_indent << "Action group valid";
      return;
    } else {
      this->getSimulator()->log() << add_indent << "Action group failed";
    }
    std::string indent = std::string(add_indent) + "   ";
    for (auto &action : actions_) {
      if (!action->isValid()) {
        action->describeState(indent.c_str());
      }
    }
  }

  virtual void setReport(const Report_ *report) override {
    for (auto &action : actions_) {
      action->setReport(report);
    }
  }

 private:

  // Bring the parent class's simulator_ member into scope
  //
  using _ActionType::valid_;

  virtual bool evalInternal() override {
    valid_ = true;

    for (auto &action : actions_) {
      valid_ &= action->eval();
    }

    return valid_;
  }

 protected:

  std::vector<std::shared_ptr<_ActionType>> actions_;
};

/// @brief Groups multiple actions.
/// @details The Grouped action only passes if all of its members pass.
///
template<typename _ActionType>
class Grouped {
 public:

  /// @brief Constructor.
  /// @tparam action_ptrs A list of actions to group.
  ///
  template<typename..._Actions>
  Grouped(_Actions...actions)
    :  Grouped<_ActionType>(DelegateConstruction{}, std::forward<_Actions>(actions)...)
  {}

  /// @brief Constructor.
  /// @tparam actions A vector with actions to group.
  ///
  Grouped(const std::vector<std::shared_ptr<_ActionType>> &actions)
    :  Grouped<_ActionType>(DelegateConstruction{}, actions)
  {}

 private:

  class Action : public GroupedAction_<_ActionType> {

   public:

    template<typename..._Actions>
    Action(_Actions...actions)
      :  GroupedAction_<_ActionType> {
      std::forward<_Actions>(actions)...
    }
    {}
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(Grouped<_ActionType>)
};

/// @brief Groups multiple generic report actions.
/// @details The Grouped action only passes if all of its members pass.
///
template<>
class Grouped<ReportAction_> {
 public:

  /// @brief Constructor.
  /// @tparam action_ptrs A list of actions to group.
  ///
  template<typename..._Actions>
  Grouped(_Actions...actions)
    :  Grouped<ReportAction_>(DelegateConstruction{}, std::forward<_Actions>(actions)...)
  {}

  /// @brief Constructor.
  /// @tparam actions A vector with actions to group.
  ///
  Grouped(const std::vector<std::shared_ptr<ReportAction_>> &actions)
    :  Grouped<ReportAction_>(DelegateConstruction{}, actions)
  {}

 private:

  class Action : public GroupedAction_<ReportAction_> {
   public:

    template<typename..._Actions>
    Action(_Actions...actions)
      :  GroupedAction_<ReportAction_> {
      std::forward<_Actions>(actions)...
    }
    {
      this->determineGroupType();
    }

    virtual uint8_t getReportTypeId() const {
      return report_type_;
    }

   private:

    void determineGroupType();

   private:

    uint8_t report_type_ = AnyTypeReportTypeId;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(Grouped<ReportAction_>)
};

/// @brief Groups multiple generic report actions.
/// @param action A first action.
/// @param more_actions More actions.
///
template<typename _FirstAction, typename..._MoreActions>
auto group(const _FirstAction &action,
           _MoreActions...more_actions)
-> decltype(group_(*unwrapAction(action), unwrapAction(action), unwrapAction(more_actions)...)) {
  return group_(*unwrapAction(action), unwrapAction(action), unwrapAction(more_actions)...);
}

/// @private
///
template<typename..._MoreActions>
Grouped<Action_> group_(Action_ &action_dummy, const std::shared_ptr<Action_> &action,
                        _MoreActions...more_actions) {
  return Grouped<Action_> {action,
                           std::forward<_MoreActions>(more_actions)...
                          };
}

/// @private
///
template<typename..._MoreActions>
Grouped<ReportAction_> group_(ReportAction_ &action_dummy, const std::shared_ptr<ReportAction_> &action,
                              _MoreActions...more_actions) {
  return Grouped<ReportAction_> {action,
                                 std::forward<_MoreActions>(more_actions)...
                                };
}

} // namespace actions
} // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
