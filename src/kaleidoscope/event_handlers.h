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

#define _CURRENT_IMPLEMENTATION

//******************************************************************************
//******************************************************************************
// Important
//******************************************************************************
//******************************************************************************
// When adding new handlers or handler versions, don't forget
// to add them both to the _FOR_EACH_EVENT_HANDLER and
// the _PROCESS_EVENT_HANDLER_VERSIONS function macros.
//******************************************************************************
//******************************************************************************

// The following properties are passed to each invocation of the OPERATION
// macro of the below definition of macro function _FOR_EACH_EVENT_HANDLER:
//
// handler name:
//    This is the name that must be used to implement a handler in a plugin.
//
// handler version:
//    Sometimes changes to the firmware's implementation require handler
//    call signatures (the list of parameters) to be adapted. To allow
//    for a deprecation periode to avoid breaking user code, it is
//    desirable to have multiple overloads of a handler with the same
//    name at the same time. Every overload must be assigned a version
//    number and every individual version must appear in the below definition
//    of the macros _FOR_EACH_EVENT_HANDLER and _PROCESS_EVENT_HANDLER_VERSIONS.
//
// deprecation flag:
//    If a specific version is meant to be deprecated, pass the DEPRECATED(...)
//    macro with a meaningful deprecation message. Pass _CURRENT_IMPLEMENTATION
//    otherwise.
//
// abortable flag:
//    Plugins' event handlers are called in the same order as the plugins
//    are passed to the KALEIDOSCOPE_INIT_PLUGINS(...) macro within the
//    sketch. For some handlers it is desirable to not call subsequent
//    plugins' handlers once a plugin's handler returned the value
//    kaleidoscope::EventHandlerResult::EVENT_CONSUMED. To enable this
//    pass the abortable flag value _ABORTABLE, _NOT_ABORTABLE otherwise.
//
// template parameter type list:
//    The hook's template type list in parenthesis, with a trailing comma.
//      e.g. (, typename _T1, typename _T2)
//    Pass empty parenthesis if the hook is non templatized.
//
// template parameters:
//    The hook's template parameters in parenthesis, with a trailing comma.
//    The template parameter names must match the template type list.
//      e.g. (, _T1, _T2)
//    Pass empty parenthesis if the hook is non templatized.
//
// dummy template arguments:
//    Supply a list of already defined dummy types that could realistically
//    be used to instantiate the template hook. Those types are only used
//    during hook method signature checks.
//    Please add parenthesis and a trailing comma.
//      e.g. (, int, int)
//    Pass empty parenthesis if the hook is non templatized.
//
// call signature:
//    The type of arguments passed to the handlers as a comma separated
//    list in brackets. Every parameter must be named.
//
// call parameters:
//    The list of parameters as they would be passed to a call to the handler.
//    Parameter names must match the names assigned to the call arguments.

namespace kaleidoscope {

// This dummy class can be used as dummy template argument to
// be passed in the definition of template hooks.
//
class SignatureCheckDummy {};
} // namespace kaleidoscope

#define _FOR_EACH_EVENT_HANDLER(OPERATION, ...)                           __NL__ \
                                                                          __NL__ \
   OPERATION(onSetup,                                                     __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (),(),(), /* non template */                               __NL__ \
               (),(), ##__VA_ARGS__)                                      __NL__ \
                                                                          __NL__ \
   /* Called at the very start of each cycle, before gathering         */ __NL__ \
   /* events, before doing anything else.                              */ __NL__ \
   OPERATION(beforeEachCycle,                                             __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (),(),(), /* non template */                               __NL__ \
               (), (), ##__VA_ARGS__)                                     __NL__ \
                                                                          __NL__ \
   /* Function called for every non-idle key, every cycle, so it       */ __NL__ \
   /* can decide what to do with it. It can modify the key (which is   */ __NL__ \
   /* passed by reference for this reason), and decide whether         */ __NL__ \
   /* further handles should be tried. If it returns                   */ __NL__ \
   /* EventHandlerResult::OK, other handlers will also get a chance    */ __NL__ \
   /* to react to the event. If it returns anything else, Kaleidoscope */ __NL__ \
   /* will stop processing there.                                      */ __NL__ \
   OPERATION(onKeyswitchEvent,                                            __NL__ \
              1,                                                          __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
               _ABORTABLE,                                                __NL__ \
                (),(),(), /* non template */                              __NL__ \
               (Key &mappedKey, KeyAddr key_addr, uint8_t keyState),      __NL__ \
               (mappedKey, key_addr, keyState), ##__VA_ARGS__)            __NL__ \
                                                                          __NL__ \
                                                                          __NL__ \
   /* Called by an external plugin (such as Kaleidoscope-FocusSerial)  */ __NL__ \
   /* via Kaleidoscope::onFocusEvent. This is where Focus events can   */ __NL__ \
   /* be handled. The function can return EventHandlerResult::OK, and  */ __NL__ \
   /* allow other plugins to handle the same command (with the caveat  */ __NL__ \
   /* that arguments can only be parsed once), or                      */ __NL__ \
   /* EventHandlerResult::EVENT_CONSUMED, in which case no other       */ __NL__ \
   /* plugin will have a chance to react to the event.                 */ __NL__ \
   OPERATION(onFocusEvent,                                                __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
                _ABORTABLE,                                               __NL__ \
                (),(),(), /* non template */                              __NL__ \
                (const char *command),                                    __NL__ \
                (command), ##__VA_ARGS__)                                 __NL__ \
                                                                          __NL__ \
   /* Called when the layer state changes. Which layes changed are    */  __NL__ \
   /* not passed as arguments. If one needs that info, they should    */  __NL__ \
   /* track Layer.getState() themselves.                              */  __NL__ \
   OPERATION(onLayerChange,                                               __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
                _NOT_ABORTABLE,                                           __NL__ \
                (),(),(), /* non template */                              __NL__ \
                (), (), ##__VA_ARGS__)                                    __NL__ \
   /* Called when the LED mode changes. If one needs to know what      */ __NL__ \
   /* from and what to the mode changed, they should track that        */ __NL__ \
   /* themselves.                                                      */ __NL__ \
   OPERATION(onLEDModeChange,                                             __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
                _NOT_ABORTABLE,                                           __NL__ \
                (),(),(), /* non template */                              __NL__ \
                (), (), ##__VA_ARGS__)                                    __NL__ \
   /* Called before reporting our state to the host. This is the       */ __NL__ \
   /* last point in a cycle where a plugin can alter what gets         */ __NL__ \
   /* reported to the host.                                            */ __NL__ \
   OPERATION(beforeReportingState,                                        __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (),(),(), /* non template */                               __NL__ \
               (),(),##__VA_ARGS__)                                       __NL__ \
                                                                          __NL__ \
   /* Called at the very end of a cycle, after everything's            */ __NL__ \
   /* said and done.                                                   */ __NL__ \
   OPERATION(afterEachCycle,                                              __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (),(),(), /* non template */                               __NL__ \
               (),(),##__VA_ARGS__)                                       __NL__ \
                                                                          __NL__ \
   /* Called before setup to enable plugins at compile time            */ __NL__ \
   /* to explore the sketch.                                           */ __NL__ \
   OPERATION(exploreSketch ,                                              __NL__ \
             1,                                                           __NL__ \
             _CURRENT_IMPLEMENTATION,                                     __NL__ \
               _NOT_ABORTABLE,                                            __NL__ \
               (,typename _Sketch),                                       __NL__ \
               (,_Sketch),                                                __NL__ \
               (,kaleidoscope::SignatureCheckDummy),                      __NL__ \
               (),                                                        __NL__ \
               (),##__VA_ARGS__)

// The following function macro lists event handler/hook method names and
// available versions. It is used to auto-generate code that is
// related to event handlers.
//
// The operators START and END are invoked with a list of all
// available hook versions while the operator OP is invoked for every
// version individually.
//
// If necessary, add handlers as provided in the following example.
//
// Example: Lets assume that a hook fancyHook has three versions,
//          then the operator invokation would be.
//
// START(fancyHook, 1, 2, 3)
//    OP(fancyHook, 1)
//    OP(fancyHook, 2)
//    OP(fancyHook, 3)
// END(fancyHook, 1, 2, 3)
//
#define _PROCESS_EVENT_HANDLER_VERSIONS(START, OP, END, ...)            __NL__ \
   START(onSetup, 1)                                                    __NL__ \
      OP(onSetup, 1)                                                    __NL__ \
   END(onSetup, 1)                                                      __NL__ \
                                                                        __NL__ \
   START(beforeEachCycle, 1)                                            __NL__ \
      OP(beforeEachCycle, 1)                                            __NL__ \
   END(beforeEachCycle, 1)                                              __NL__ \
                                                                        __NL__ \
   START(onKeyswitchEvent, 1)                                           __NL__ \
      OP(onKeyswitchEvent, 1)                                           __NL__ \
   END(onKeyswitchEvent, 1)                                             __NL__ \
                                                                        __NL__ \
   START(onFocusEvent, 1)                                               __NL__ \
      OP(onFocusEvent, 1)                                               __NL__ \
   END(onFocusEvent, 1)                                                 __NL__ \
                                                                        __NL__ \
   START(onLayerChange, 1)                                              __NL__ \
      OP(onLayerChange, 1)                                              __NL__ \
   END(onLayerChange, 1)                                                __NL__ \
                                                                        __NL__ \
   START(onLEDModeChange, 1)                                            __NL__ \
      OP(onLEDModeChange, 1)                                            __NL__ \
   END(onLEDModeChange, 1)                                              __NL__ \
                                                                        __NL__ \
   START(beforeReportingState, 1)                                       __NL__ \
      OP(beforeReportingState, 1)                                       __NL__ \
   END(beforeReportingState, 1)                                         __NL__ \
                                                                        __NL__ \
   START(afterEachCycle, 1)                                             __NL__ \
      OP(afterEachCycle, 1)                                             __NL__ \
   END(afterEachCycle, 1)                                               __NL__ \
                                                                        __NL__ \
   START(exploreSketch, 1)                                              __NL__ \
      OP(exploreSketch, 1)                                              __NL__ \
   END(exploreSketch, 1)
