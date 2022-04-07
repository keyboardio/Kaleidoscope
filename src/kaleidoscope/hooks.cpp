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

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/event_handlers.h"        // for _FOR_EACH_EVENT_HANDLER
#include "kaleidoscope/hooks.h"                 // for Hooks
#include "kaleidoscope/macro_helpers.h"         // for __NL__, MAKE_TEMPLATE_SIGNATURE, UNWRAP

namespace kaleidoscope {

// The following weak symbols are overwritten by using the
// KALEIDOSCOPE_INIT_PLUGINS(...) macro in the firmware sketch. Their only
// purpose is to ensure that there are no undefined symbols if
// KALEIDOSCOPE_INIT_PLUGINS(...) is not used. Sketches that use no plugins do
// not use the macro, because due to technical reasons, we do not support an
// empty KALEIDOSCOPE_INIT_PLUGINS() invocation. These symbols therefore ensure
// that we can compile sketches that use no plugins, without them having to use
// KALEIDOSCOPE_INIT_PLUGINS() with a dummy plugin.

// clang-format off
#define INSTANTIATE_WEAK_HOOK_FUNCTION(                                        \
    HOOK_NAME, HOOK_VERSION, DEPRECATION_TAG,                                  \
    SHOULD_EXIT_IF_RESULT_NOT_OK,                                              \
    TMPL_PARAM_TYPE_LIST, TMPL_PARAM_LIST, TMPL_DUMMY_ARGS_LIST,               \
    SIGNATURE, ARGS_LIST)                                               __NL__ \
                                                                        __NL__ \
   MAKE_TEMPLATE_SIGNATURE(UNWRAP TMPL_PARAM_TYPE_LIST)                 __NL__ \
   __attribute__((weak))                                                __NL__ \
   EventHandlerResult Hooks::HOOK_NAME SIGNATURE {                      __NL__ \
      return EventHandlerResult::OK;                                    __NL__ \
   }

_FOR_EACH_EVENT_HANDLER(INSTANTIATE_WEAK_HOOK_FUNCTION)
// clang-format on

#undef INSTANTIATE_WEAK_HOOK_FUNCTION

namespace sketch_exploration {
class Sketch;
}

// Make sure that there is no undefined symbol if KALEIDOSCOPE_INIT_PLUGINS(...)
// is not invoked in the user's sketch.
//
template<>
__attribute__((weak))
EventHandlerResult
Hooks::exploreSketch<sketch_exploration::Sketch>() {
  return EventHandlerResult::OK;
}

}  // namespace kaleidoscope
