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

#include <algorithm>

namespace kaleidoscope {
namespace simulator {
namespace interface {
    namespace actions {

/// @brief Overload this function if you intent to use any other data type
///        that can be mapped to a keycode.
///
    inline
    uint8_t toModifier(uint8_t modifier) {
      return modifier;
    }

/// @brief Asserts that a specific list of modifiers is active in the keyboard report.
///
    class AssertModifiersActive {

     public:

      /// @brief Constructor.
      /// @param modifiers A list of modifier keycodes.
      /// @param exclusively If true only the listed modifiers may
      ///                  be part of the keyboard report for the
      ///                  action to pass.
      ///
      AssertModifiersActive(const std::vector<uint8_t> &modifiers,
                            bool exclusively = false)
        :  AssertModifiersActive(DelegateConstruction{}, modifiers, exclusively)
      {}

      /// @brief Constructor.
      /// @tparam key_info A list of key information values. Those
      ///        may be a mixture of keycodes or Key values.
      ///
      template<typename..._KeyInfo>
      AssertModifiersActive(_KeyInfo...key_info)
        :  AssertModifiersActive(DelegateConstruction{}, std::forward<_KeyInfo>(key_info)...)
      {}

      /// @details After this was called, no other modifiers than the listed
      ///          ones are allowed in the keyboard report for the
      ///          action to pass.
      ///
      void exclusively() {
        action_->setExclusively(true);
      }

     private:

      class Action : public ReportAction<KeyboardReport_> {

       public:

        virtual ~Action() = default;


        Action(const std::vector<uint8_t> &modifiers,
               bool exclusively = false)
          :  modifiers_(modifiers),
             exclusively_(exclusively)
        {}

        template<typename..._KeyInfo>
        Action(_KeyInfo...key_info)
          :  modifiers_{toModifier(std::forward<_KeyInfo>(key_info))...},
             exclusively_(false)
        {}

        virtual void describe(const char *add_indent = "") const override {
          this->getSimulator()->log() << add_indent << "Modifiers active: ";

          if (modifiers_.empty()) {
            this->getSimulator()->log() << "<none>";
            return;
          }

          for (auto modifier : modifiers_) {
            this->getSimulator()->log()
                << this->getSimulator()->getCore().keycodeToName(modifier) << " ";
          }
        }

        virtual void describeState(const char *add_indent = "") const override {

          this->getSimulator()->log() << add_indent << "Modifiers actually active: ";

          auto active_modifiers = this->getReport().getActiveModifiers();

          if (active_modifiers.empty()) {
            this->getSimulator()->log() << "<none>";
            return;
          }
          for (auto modifier : active_modifiers) {
            this->getSimulator()->log()
                << this->getSimulator()->getCore().keycodeToName(modifier) << " ";
          }
        }

        virtual bool evalInternal() override {

          for (auto modifier : modifiers_) {
            if (!this->getReport().isModifierKeycodeActive(modifier)) {
              return false;
            }
          }

          if (exclusively_) {

            auto active_modifiers = this->getReport().getActiveModifiers();

            for (auto modifier : active_modifiers) {

              if (std::find(modifiers_.begin(), modifiers_.end(), modifier) == modifiers_.end()) {
                return false;
              }
            }
          }

          return true;
        }

        /// @brief Set exclusivity of the modifiers allowed in the keyboard
        ///        report.
        /// @param state The exclusivity state.
        ///
        void setExclusively(bool state) {
          exclusively_ = state;
        }

        /// @brief Retreives the state of exclusivity of modifiers in the
        ///        keyboard report.
        /// @return [bool] The exclusivity state.
        ///
        bool getExclusively() const {
          return exclusively_;
        }

       private:

        std::vector<uint8_t> modifiers_;
        bool exclusively_ = false;
      };

      SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY(AssertModifiersActive)
    };

    } // namespace actions
  } // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
