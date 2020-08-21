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

#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace simulator {
namespace executor {
namespace actions {

/// @brief Asserts that a given layer is the current top layer.
///
class AssertTopActiveLayerIs {

 public:

  /// @brief Constructor.
  /// @param[in] layer_id The id of the layer to check as top active.
  ///
  AssertTopActiveLayerIs(int layer_id)
    : AssertTopActiveLayerIs(DelegateConstruction{}, layer_id)
  {}

 private:

  class Action : public framework::Action_ {

   public:

    Action(int layer_id) : layer_id_(layer_id) {}

    virtual void describe(const char *add_indent = "") const override {
      this->getSimulator()->log() << add_indent << "Top active layer is " << layer_id_;
    }

    virtual void describeState(const char *add_indent = "") const {
      this->getSimulator()->log() << add_indent << "Top active layer is " << Layer.top();
    }

    virtual bool evalInternal() override {
      return Layer.top() == (uint8_t)layer_id_;
    }

   private:

    int layer_id_;
  };

  SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY(AssertTopActiveLayerIs)
};

} // namespace actions
} // namespace executor
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
