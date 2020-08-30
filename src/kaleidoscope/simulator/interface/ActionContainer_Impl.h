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

#include "kaleidoscope/simulator/interface/ActionContainer.h"
#include "kaleidoscope/simulator/interface/actions/Grouped.h"
#include "kaleidoscope/simulator/interface/aux/demangle.h"

// Undefine some macros that are defined by Arduino
#undef min
#undef max

#include <deque>
#include <vector>
#include <memory>

namespace kaleidoscope {
namespace simulator {
namespace interface {

    template<typename _ActionType>
    ActionContainer<_ActionType> &
    ActionContainer<_ActionType>
    ::add(const std::shared_ptr<_ActionType> &action) {
      this->configureAction(action);
      container_.push_back(action);
      simulator_.log() << "Adding " << _ActionType::typeString() << " action: "
                       << aux::type(*action);
      return *this;
    }

    template<typename _ActionType>
    ActionContainer<_ActionType> &
    ActionContainer<_ActionType>
    ::add(const std::vector<std::shared_ptr<_ActionType>> &actions) {
      for (auto &action : actions) {
        this->add(action);
      }
      return *this;
    }

    template<typename _ActionType>
    ActionContainer<_ActionType> &
    ActionContainer<_ActionType>
    ::addGrouped(const std::vector<std::shared_ptr<_ActionType>> &actions) {
      for (auto &action : actions) {
        this->configureAction(action);
      }

      std::shared_ptr<_ActionType> grouped_actions
        = std::static_pointer_cast<_ActionType>(
            actions::Grouped<_ActionType> {actions} .ptr()
          );

      this->configureAction(grouped_actions);

      this->configureAction(grouped_actions);
      container_.push_back(grouped_actions);

      simulator_.log() << "Adding grouped " << _ActionType::typeString() << " actions";
      for (const auto &action : actions) {
        simulator_.log() << "   " << aux::type(*action);
      }

      return *this;
    }

    template<typename _ActionType>
    ActionContainer<_ActionType> &
    ActionContainer<_ActionType>
    ::removeInternal(const std::shared_ptr<_ActionType> &action) {
      bool remove_success = false;

      for (auto iter = container_.begin(); iter != container_.end() ;) {
        if (*iter == action) {
          iter = container_.erase(iter);
          remove_success = true;
          break;
        } else {
          ++iter;
        }
      }
      if (remove_success) {
        simulator_.log() << "Removed " << _ActionType::typeString() << " action: "
                         << aux::type(*action);
      } else {
        simulator_.error() << "Failed to remove " << _ActionType::typeString() << " action: "
                           << aux::type(*action);
      }
      return *this;
    }

    template<typename _ActionType>
    ActionContainer<_ActionType> &
    ActionContainer<_ActionType>
    ::remove(const std::vector<std::shared_ptr<_ActionType>> &actions) {
      for (auto &action : actions) {
        this->remove(action);
      }
      return *this;
    }

    template<typename _ActionType>
    std::shared_ptr<_ActionType>
    ActionContainer<_ActionType>
    ::popFront() {
      auto front_element = container_.front();
      container_.pop_front();
      return front_element;
    }

    template<typename _ActionType>
    void
    ActionContainer<_ActionType>
    ::configureAction(const std::shared_ptr<_ActionType> &action) {
      action->setSimulator(&simulator_);
    }

  } // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
