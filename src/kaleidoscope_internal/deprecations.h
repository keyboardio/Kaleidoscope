/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2020  Keyboard.io, Inc.
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

#include "kaleidoscope/macro_helpers.h"

#define DEPRECATED(tag)                                                 \
  __attribute__((deprecated(_DEPRECATE(_DEPRECATED_MESSAGE_ ## tag))))

#define _DEPRECATE(message) "\n"                                               \
  "------------------------------------------------------------------------\n" \
  message                                                                      \
  "\n"                                                                         \
  "------------------------------------------------------------------------\n" \

/* Messages */

#define _DEPRECATED_MESSAGE_HANDLE_KEYSWITCH_EVENT                            __NL__ \
  "`handleKeyswitchEvent()` has been deprecated.\n"                           __NL__ \
  "Please use `Runtime.handleKeyEvent()` instead.\n"                          __NL__ \
  "This function will be removed after 2021-08-01."

#define _DEPRECATED_MESSAGE_ON_KEYSWITCH_EVENT_V1                             __NL__ \
  "The `onKeyswitchEvent()` event handler is deprecated.\n"                   __NL__ \
  "Please replace it with an `onKeyEvent()` handler. See the documentation\n" __NL__ \
  "in UPGRADING.md and docs/api-reference/event-handler-hooks.md for more\n"  __NL__ \
  "information on what changes are needed to adapt old plugins to the new\n"  __NL__ \
  "event handler API.\n"                                                      __NL__ \
  "This function will be removed after 2021-08-01."

#define _DEPRECATED_MESSAGE_BEFORE_REPORTING_STATE_V1                          __NL__ \
  "This `beforeReportingState()` event handler version is deprecated.\n"       __NL__ \
  "There is a new `beforeReportingState(KeyEvent)` handler that can be used\n" __NL__ \
  "instead, for plugins that need to execute code before each new HID\n"       __NL__ \
  "report is sent. However, the new handler does not run every cycle, but\n"   __NL__ \
  "only in response to key events. If you have code that is intended to run\n" __NL__ \
  "every scan cycle, it should be moved to the `afterEachCycle()` event\n"     __NL__ \
  "handler instead.\n"                                                         __NL__ \
  "This function will be removed after 2021-08-01."
