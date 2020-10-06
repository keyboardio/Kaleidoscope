/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#include <Arduino.h>

namespace kaleidoscope {
class Key;
}

#include "kaleidoscope/KeyAddr.h"
#include "kaleidoscope/plugin.h"
#include "kaleidoscope/event_handlers.h"

// Forward declaration required to enable friend declarations
// in class Hooks.
class kaleidoscope_;
extern void handleKeyswitchEvent(kaleidoscope::Key mappedKey, KeyAddr key_addr, uint8_t keyState);

namespace kaleidoscope {
namespace plugin {
// Forward declaration to enable friend declarations.
class LEDControl;
}

// Forward declaration to enable friend declarations.
class Layer_;

namespace sketch_exploration {
void pluginsExploreSketch();
} // namespace sketch_exploration

// The reason why the hook routing entry point functions live within
// class Hooks and not directly within a namespace is, that we want
// to restrict who is allowed to trigger hooks, mainly to prevent
// user code from calling hook methods.
//
// A note to maintainers: When you add new hooks that are supposed to
// be called from other places than the friend classes and functions listed
// below, just add a suitable friend declaration.

class Hooks {

  // The following friend declarations restrict access to
  // the hook routing system.

  // Runtime_ calls Hooks::onSetup, Hooks::beforeReportingState
  // and Hooks::afterEachCycle.
  friend class Runtime_;
  friend class ::kaleidoscope::Layer_;
  friend class ::kaleidoscope::plugin::LEDControl;
  friend void ::kaleidoscope::sketch_exploration::pluginsExploreSketch();

  // ::handleKeyswitchEvent(...) calls Hooks::onKeyswitchEvent.
  friend void ::handleKeyswitchEvent(kaleidoscope::Key mappedKey,
                                     KeyAddr key_addr, uint8_t keyState);

 private:

  // The following private functions are just to be called by classes
  // and functions that are declared as friends above.

#define DEFINE_WEAK_HOOK_FUNCTION(                                             \
    HOOK_NAME, HOOK_VERSION, DEPRECATION_TAG,                                  \
    SHOULD_ABORT_ON_CONSUMED_EVENT,                                            \
    TMPL_PARAM_TYPE_LIST, TMPL_PARAM_LIST, TMPL_DUMMY_ARGS_LIST,               \
    SIGNATURE, ARGS_LIST)                                               __NL__ \
                                                                        __NL__ \
   MAKE_TEMPLATE_SIGNATURE(UNWRAP TMPL_PARAM_TYPE_LIST)                 __NL__ \
   static EventHandlerResult HOOK_NAME SIGNATURE;

  _FOR_EACH_EVENT_HANDLER(DEFINE_WEAK_HOOK_FUNCTION)

#undef DEFINE_WEAK_HOOK_FUNCTION
};

}
