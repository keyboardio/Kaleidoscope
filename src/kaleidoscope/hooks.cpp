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

#include "kaleidoscope/hooks.h"

namespace kaleidoscope {

// The following weak symbols are overwritten by
// using the KALEIDOSCOPE_INIT_PLUGINS(...) macro
// in the the firmware sketch. Their only purpose is
// to provide backwards compatibility during the transition
// from the old implementation of the hooking/plugin system
// to the new one. The weak symbols ensure that there
// are no undefined symbols if KALEIDOSCOPE_INIT_PLUGINS(...)
// is not used. This allows legacy sketches to be used
// during the transition phase.

#define INSTANTIATE_WEAK_HOOK_FUNCTION(                                        \
    HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST)    __NL__ \
                                                                        __NL__ \
   __attribute__((weak))                                                __NL__ \
   EventHandlerResult Hooks::HOOK_NAME SIGNATURE {                              __NL__ \
      return EventHandlerResult::OK;                                    __NL__ \
   }

_FOR_EACH_EVENT_HANDLER(INSTANTIATE_WEAK_HOOK_FUNCTION)

#undef INSTANTIATE_WEAK_HOOK_FUNCTION

} // namespace kaleidoscope
