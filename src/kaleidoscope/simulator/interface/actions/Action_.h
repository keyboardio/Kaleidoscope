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

#undef min
#undef max

#include <string>
#include <ostream>
#include <memory>

namespace kaleidoscope {
namespace simulator {
namespace interface {

    class Simulator_;
    class Report_;

/// @private
///
    class VoidReport {};

/// @brief An abstract action.
/// @details This abstract class serves as base class for any
///        actions.
///
///        **Important:** This class is not part of Papilio's
///                   public API. It is meant for internal use only.
///
    class Action_ {

     public:

      typedef VoidReport ReportType;
      typedef Action_ ActionBaseType;

      /// @brief Reports information about the action.
      ///
      /// @details Do not override this method but the method describeState instead.
      ///
      /// @param add_indent An additional indentation string.
      ///
      virtual void report(const char *add_indent = "") const;

      /// @brief Describes the action.
      ///
      /// @param add_indent An additional indentation string.
      ///
      virtual void describe(const char *add_indent = "") const = 0;

      /// @brief Evaluates the condition that is supposed to be asserted.
      /// @details Do not override this method.
      ///
      /// @return [bool] True if the action passed, false otherwise.
      ///
      bool eval() {
        valid_ = (this->evalInternal() != negate_); // logical XOR
        return valid_;
      }

      /// @brief Describes the current state of the action object.
      /// @details Possibly provides information about what went wrong
      ///         if the action failed.
      ///
      /// @param add_indent An additional indentation string.
      ///
      ///
      virtual void describeState(const char *add_indent = "") const;

      /// @brief Register the test simulator with the action.
      ///
      virtual void setSimulator(const Simulator_ *simulator) {
        simulator_ = simulator;
      }

      /// @brief Retreive the test simulator object that is associated with
      ///        the action.
      ///
      /// @return A pointer to the current test simulator.
      ///
      const Simulator_ *getSimulator() const {
        return simulator_;
      }

      /// @brief Checks validity of an action.
      ///
      /// @return True if valid, false otherwise.
      ///
      bool isValid() const {
        return valid_;
      }

      /// @brief Set the boolean negation state of the action validity check.
      /// @details If negation is enabled, an action is assumed to be correct
      ///        if the non negated version would fail.
      ///
      /// @param state The negation state.
      ///
      void negate(bool state = true) {
        negate_ = state;
      }

      /// @brief Set the boolean negation state of the action validity check.
      /// @details If negation is enabled, an action is assumed to be correct
      ///        if the non negated version would fail.
      ///
      /// @param state The negation state.
      ///
      void setNegate(bool state = true) {
        negate_ = state;
      }

      /// @brief Retreive the boolean negation state of the action validity check.
      ///
      /// @return The negation state.
      ///
      bool getNegate() const {
        return negate_;
      }

      /// @details This noop method is there to satisfy the interface
      ///        of derived classes. It is not meant to be called.
      ///
      virtual void setReport(const Report_ *report) {}

      static const char *typeString() {
        return "generic";
      }

      virtual const char *getTypeString() const {
        return typeString();
      }

     protected:

      /// @brief Evaluates the condition that is supposed to be asserted.
      /// @details Override this method in derived actions.
      ///
      /// @return [bool] True if the action passed, false otherwise.
      ///
      virtual bool evalInternal() = 0;

     protected:

      bool valid_ = false;
      const Simulator_ *simulator_ = nullptr;

     private:

      bool negate_ = false;
    };

    template<typename _T>
    std::shared_ptr<_T> unwrapAction(std::shared_ptr<_T> &&ptr) {
      return std::forward(ptr);
    }

    template<typename _T>
    std::shared_ptr<_T> unwrapAction(const std::shared_ptr<_T> &ptr) {
      return ptr;
    }

    template<typename _T>
    auto unwrapAction(const _T &action) -> decltype(action.ptr()) {
      return action.ptr();
    }

  } // namespace interface
} // namespace simulator
} // namespace kaleidoscope

#define SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_(WRAPPER, TYPENAME_KEYWORD)     \
                                                                               \
   private:                                                                    \
      std::shared_ptr<WRAPPER::Action> action_;                                \
                                                                               \
   public:                                                                     \
      struct DelegateConstruction {};                                          \
                                                                               \
      template<typename..._Args>                                               \
      WRAPPER(DelegateConstruction, _Args...args)                              \
         : action_{new WRAPPER::Action{std::forward<_Args>(args)...}}          \
      {}                                                                       \
                                                                               \
      /*operator std::shared_ptr<TYPENAME_KEYWORD WRAPPER::Action::ActionBaseType> () { return action_; }                                                           \
      operator std::shared_ptr<WRAPPER::Action> () const { return action_; } */ \
      std::shared_ptr<WRAPPER::Action> ptr() const { return action_; }         \
                                                                               \
      /*std::shared_ptr<TYPENAME_KEYWORD WRAPPER::Action::ActionBaseType> ptr() { return action_; }                                                           */\
                                                                               \
      WRAPPER &negate(bool state = true) { action_->negate(state); return *this; }

#define SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY(WRAPPER)                        \
   SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_(WRAPPER,)

#define SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_TMPL(WRAPPER)                   \
   SIMULATOR_AUTO_DEFINE_ACTION_INVENTORY_(WRAPPER, typename)

#define SIMULATOR_ACTION_STD_CONSTRUCTOR(WRAPPER, ...)                         \
      WRAPPER()                                                                \
         : WRAPPER(DelegateConstruction{})                                     \
      {                                                                        \
         __VA_ARGS__                                                           \
      }

#endif // KALEIDOSCOPE_VIRTUAL_BUILD
