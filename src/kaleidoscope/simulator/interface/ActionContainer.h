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

class Simulator_;

/// @brief An auxiliary class that represents a container of action objects.
///
template<typename _ActionType>
class ActionContainer {
 public:

  typedef ActionContainer<_ActionType> ThisType;
  typedef _ActionType ActionType;

  /// @brief Constructor.
  /// @param simulator The associated Simulator object.
  ///
  ActionContainer(Simulator_ &simulator)
    :  simulator_(simulator)
  {}

  /// @brief Adds an action.
  ///
  /// @param actions The actions to be added to the container.
  ///
  ThisType &add(const std::shared_ptr<_ActionType> &action);

  /// @brief Adds actions.
  ///
  /// @param actions The actions to be added to the container.
  ///
  ThisType &add(const std::vector<std::shared_ptr<_ActionType>> &actions);

  /// @brief Adds actions.
  ///
  /// @tparam actions The actions to be added to the container.
  ///
  template<typename..._Actions>
  ThisType &add(_Actions...actions) {
    this->add(
    std::vector<std::shared_ptr<_ActionType>> {
      unwrapAction(actions)...
    }
    );
    return *this;
  }

  /// @brief Adds a list of actions.
  ///
  /// @param actions The actions to be added to the container.
  ///
  ThisType &addGrouped(const std::vector<std::shared_ptr<_ActionType>> &actions);

  /// @brief Adds a list of actions.
  ///
  /// @tparam actions The actions to be added to the container.
  ///
  template<typename..._Actions>
  ThisType &addGrouped(_Actions...actions) {
    return this->addGrouped(
    std::vector<std::shared_ptr<_ActionType>> {
      unwrapAction(actions)...
    }
           );
    return *this;
  }

  /// @brief Removes an action.
  ///
  /// @param action The action to be removed from the container.
  ///
  template<typename _T>
  ThisType &remove(const _T &action) {
    return this->removeInternal(unwrapAction(action));
  }

  /// @brief Removes a list of actions.
  ///
  /// @param actions The actions to be removed from the container.
  ///
  ThisType &remove(const std::vector<std::shared_ptr<_ActionType>> &actions);

  /// @brief Removes the action at the head of the container and returns it.
  ///
  /// @returns The former front element of the container.
  ///
  std::shared_ptr<_ActionType> popFront();

  /// @brief Retreives the number of entries in the container.
  ///
  std::size_t size() const {
    return container_.size();
  }
  /// @brief Checks the container for being empty.
  ///
  /// @returns True if the container is empty, false otherwise.
  ///
  bool empty() const {
    return container_.empty();
  }

  /// @brief Clear the entire container.
  ///
  void clear() {
    container_.clear();
  }

  /// @brief Enables direct access to the undelying std::deque object.
  ///
  const std::deque<std::shared_ptr<_ActionType>> &directAccess() {
    return container_;
  }

 private:

  void configureAction(const std::shared_ptr<_ActionType> &action);

  /// @brief Removes an action.
  ///
  /// @param action The action to be removed from the container.
  ///
  ThisType &removeInternal(const std::shared_ptr<_ActionType> &action);

 private:

  Simulator_ &simulator_;
  std::deque<std::shared_ptr<_ActionType>> container_;
};

} // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
