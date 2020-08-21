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

#include "kaleidoscope/simulator/interface/actions/generic_report/ReportAction.h"
#include "kaleidoscope/simulator/interface/SimulatorCore_.h"

namespace kaleidoscope {
namespace simulator {
namespace interface {
    namespace actions {

/// @brief Overload this function if you intent to use any other data type
///        that can be mapped to a keycode.
///
    inline
    uint8_t toKeycode(uint8_t keycode) {
      return keycode;
    }

/// @brief Asserts that a specific set of keys is active in the keyboard report.
///
    class AssertKeycodesActive {

     public:

      /// @brief Constructor.
      ///
      /// @param keycodes A collection of keycodes.
      /// @param exclusively If enabled no other keys than the listed
      ///                 ones must be part of the checked keyboard report.
      ///
      AssertKeycodesActive(const std::vector<uint8_t> &keycodes,
                           bool exclusively = false)
        : AssertKeycodesActive(DelegateConstruction{}, keycodes, exclusively)
      {}

      /// @brief Constructor.
      ///
      /// @tparam key_info A list of key information objects. This
      ///         can be a mixture of keycodes or Key specifications.
      ///
      template<typename..._KeyInfo>
      AssertKeycodesActive(_KeyInfo...key_info)
        : AssertKeycodesActive(DelegateConstruction{}, std::forward<_KeyInfo>(key_info)...)
      {}

      /// @details After this was called, no other keys than the listed
      ///          ones are allowed in the keyboard report for the
      ///          action to pass.
      ///
      void exclusively() {
        action_->setExclusively(true);
      }

     private:

      class Action : public ReportAction<KeyboardReport_> {

       public:

        Action(const std::vector<uint8_t> &keycodes,
               bool exclusively = false)
          :  keycodes_(keycodes),
             exclusively_(exclusively)
        {}

        template<typename..._KeyInfo>
        Action(_KeyInfo...key_info)
          :  keycodes_{toKeycode(std::forward<_KeyInfo>(key_info))...},
             exclusively_(false)
        {}

        virtual void describe(const char *add_indent = "") const override {
          this->getSimulator()->log() << add_indent << "Keycodes active: ";

          if (keycodes_.empty()) {
            this->getSimulator()->log() << "<none>";
            return;
          }

          for (auto keycode : keycodes_) {
            this->getSimulator()->log()
                << this->getSimulator()->getCore().keycodeToName(keycode) << " ";
          }
        }

        virtual void describeState(const char *add_indent = "") const override {

          this->getSimulator()->log() << add_indent << "Keycodes actually active: ";

          auto active_keycodes = this->getReport().getActiveKeycodes();

          if (active_keycodes.empty()) {
            this->getSimulator()->log() << "<none>";
            return;
          }
          for (auto keycode : active_keycodes) {
            this->getSimulator()->log()
                << this->getSimulator()->getCore().keycodeToName(keycode) << " ";
          }
        }

        virtual bool evalInternal() override {

          for (auto keycode : keycodes_) {
            if (!this->getReport().isKeycodeActive(keycode)) {
              return false;
            }
          }

          if (exclusively_) {

            auto active_keycodes = this->getReport().getActiveKeycodes();

            for (auto keycode : active_keycodes) {

              if (std::find(keycodes_.begin(), keycodes_.end(), keycode) == keycodes_.end()) {
                return false;
              }
            }
          }

          return true;
        }

        /// @brief Set exclusivity of the keycodes allowed in the keyboard
        ///        report.
        /// @param state The exclusivity state.
        ///
        void setExclusively(bool state) {
          exclusively_ = state;
        }

        /// @brief Retreives the state of exclusivity of keycodes in the
        ///        keyboard report.
        /// @return [bool] The exclusivity state.
        ///
        bool getExclusively() const {
          return exclusively_;
        }

       private:

        std::vector<uint8_t> keycodes_;
        bool exclusively_ = false;
      };

      SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY(AssertKeycodesActive)
    };

    } // namespace actions
  } // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
