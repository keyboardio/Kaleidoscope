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

// This file defines the names and argument signatures for all event handlers
// in the Kaleidoscope core.
//
// When adding a new hook or event handler to Kaleidoscope, it needs only
// to be added here and to the place in the codebase where it should be called.
//
// _FOR_EACH_EVENT_HANDLER is what's called an "X Macro". Its first argument,
// OPERATION, is the name of another macro. When called, it will execute
// OPERATION once for each event handler defined herein with the following
// parameters:
//
// HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST, ...
//
// Any additional parameters that are added to an invokation
// of _FOR_EACH_EVENT_HANDLER are passed through to OP.

#define _ABORTABLE true
#define _NOT_ABORTABLE false

#define _FOR_EACH_EVENT_HANDLER(OPERATION, ...)                           __NL__ \
                                                                          __NL__ \
   OPERATION(onSetup,                                                     __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (),(), ##__VA_ARGS__)                                      __NL__ \
                                                                          __NL__ \
   /* Called at the very start of each cycle, before gathering         */ __NL__ \
   /* events, before doing anything else.                              */ __NL__ \
   OPERATION(beforeEachCycle,                                             __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (), (), ##__VA_ARGS__)                                     __NL__ \
                                                                          __NL__ \
   /* DEPRECATED                                                       */ __NL__ \
   /* Function called for every non-idle key, every cycle, so it       */ __NL__ \
   /* can decide what to do with it. It can modify the key (which is   */ __NL__ \
   /* passed by reference for this reason), and decide whether         */ __NL__ \
   /* further handles should be tried. If it returns                   */ __NL__ \
   /* EventHandlerResult::OK, other handlers will also get a chance    */ __NL__ \
   /* to react to the event. If it returns anything else, Kaleidoscope */ __NL__ \
   /* will stop processing there.                                      */ __NL__ \
   OPERATION(onKeyswitchEvent,                                            __NL__ \
               _ABORTABLE,                                                __NL__ \
               (Key &mappedKey, byte row, byte col, uint8_t keyState),    __NL__ \
               (mappedKey, row, col, keyState), ##__VA_ARGS__)            __NL__ \
                                                                          __NL__ \
   /* Function called for every non-idle key, every cycle, so it       */ __NL__ \
   /* can decide what to do with it. It can modify the key (which is   */ __NL__ \
   /* passed by reference for this reason), and decide whether         */ __NL__ \
   /* further handles should be tried. If it returns                   */ __NL__ \
   /* EventHandlerResult::OK, other handlers will also get a chance    */ __NL__ \
   /* to react to the event. If it returns anything else, Kaleidoscope */ __NL__ \
   /* will stop processing there.                                      */ __NL__ \
   OPERATION(onKeyswitchEvent2,                                           __NL__ \
               _ABORTABLE,                                                __NL__ \
               (Key &mappedKey, KeyAddr key_addr, uint8_t keyState),       __NL__ \
               (mappedKey, key_addr, keyState), ##__VA_ARGS__)             __NL__ \
                                                                          __NL__ \
   /* Called by an external plugin (such as Kaleidoscope-FocusSerial)  */ __NL__ \
   /* via Kaleidoscope::onFocusEvent. This is where Focus events can   */ __NL__ \
   /* be handled. The function can return EventHandlerResult::OK, and  */ __NL__ \
   /* allow other plugins to handle the same command (with the caveat  */ __NL__ \
   /* that arguments can only be parsed once), or                      */ __NL__ \
   /* EventHandlerResult::EVENT_CONSUMED, in which case no other       */ __NL__ \
   /* plugin will have a chance to react to the event.                 */ __NL__ \
   OPERATION(onFocusEvent,                                                __NL__ \
                _ABORTABLE,                                               __NL__ \
                (const char *command),                                    __NL__ \
                (command), ##__VA_ARGS__)                                 __NL__ \
                                                                          __NL__ \
   /* Called when the layer state changes. Which layes changed are    */  __NL__ \
   /* not passed as arguments. If one needs that info, they should    */  __NL__ \
   /* track Layer.getState() themselves.                              */  __NL__ \
   OPERATION(onLayerChange,                                               __NL__ \
                _NOT_ABORTABLE,                                           __NL__ \
                (), (), ##__VA_ARGS__)                                    __NL__ \
   /* Called before reporting our state to the host. This is the       */ __NL__ \
   /* last point in a cycle where a plugin can alter what gets         */ __NL__ \
   /* reported to the host.                                            */ __NL__ \
   OPERATION(beforeReportingState,                                        __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (),(),##__VA_ARGS__)                                       __NL__ \
                                                                          __NL__ \
   /* Called at the very end of a cycle, after everything's            */ __NL__ \
   /* said and done.                                                   */ __NL__ \
   OPERATION(afterEachCycle,                                              __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (),(),##__VA_ARGS__)
