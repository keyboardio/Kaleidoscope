/*
 * This file contains pre-processor macros, which are not namespaced, the
 * vast majority of code these macros generate will live under the
 * kaleidoscope_internal namespace. That is why this header is here.
 *
 * The only exception to this is the _KALEIDOSCOPE_INIT_PLUGINS macro, which
 * also places a few symbols under the kaleidoscope namespace. But that code is
 * tightly coupled with the other parts, which are in kaleidoscope_internal.
 *
 * The reason we use an entirely separate namespace is that
 * Some plugins' classes are in the kaleidoscope namespace, and the instances
 * of those classes have the same names as the classes, but in the global
 * namespace. In these cases, the macros herein would resolve to the classes,
 * when we want them to resolve to instances. To avoid this, we put the
 * generated code in an entirely different namespace.
 */

#pragma once

#include "macro_helpers.h"
#include "plugin.h"
#include "kaleidoscope/hooks.h"
#include "eventhandler_signature_check.h"

// A note to maintainers: How to add additional hooks
//
// 1) Add an appropriate non-virtual (important!) method
//       to class kaleidoscope::Plugin.
// 2) Add a static method with the same name and call signature
//       to class kaleidoscope::Hooks.
// 3) Add an implemenation of the new static method of kaleidoscope::Hooks
//       introduced in 2) to the end of macro KALEIDOSCOPE_INIT_PLUGINS(...)
//       and a matching weak implementation to kaleidoscope/hooks.cpp

// Some words about the design of hook routing:
//
// The EventDispatcher class implements a compile-time loop over all plugins, which
// calls an event handler on each plugin.
//
// Each hook called by the firmware core compiles down to only a single
// static function call (in class Hooks), rather than one for each plugin.
//
// This approach is better than using virtual event handlers in classes
// derived from kaleidoscope::Plugin because it results in significantly fewer
// virtual function calls and virtual function tables (vtables).
//
// The call to event handlers through kaleidoscope::Hooks and
// kaleidoscope_internal::EventDispatcher is templated to allow for compile time
// static polymorphisms (plugins' EventHandlers aren't virtual).
//
// Technically, plugins don't need to be derived from kaleidoscope::Plugin, but
// only need to implement a set of event handlers whose call signature match the
// API definition. This approach is similar to duck typing in scripting languages:
// The compiler only cares that a method's signature matches, not that the plugin
// is derived from kaleidoscope::Plugin.
//
// Static hook functions inside the Hooks class each use the EventDispatcher
// helper class to cast an associated EventHandler on each plugin instance.


#define _INLINE_EVENT_HANDLER_FOR_PLUGIN(PLUGIN)                            \
                                                                     __NL__ \
   result = EventHandler__::call(PLUGIN, hook_args...);              __NL__ \
                                                                     __NL__ \
   if (EventHandler__::shouldAbortOnConsumedEvent() &&               __NL__ \
       result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) { __NL__ \
      return result;                                                 __NL__ \
   }                                                                 __NL__


// This defines an auxiliary class 'EventHandler_Foo' for each hook 'Foo'.
// Kaleidoscope::Hooks calls the EventDispatcher class, which in turn invokes
// the event handler method 'Foo' of each registered plugin with a given
// set of arguments.


#define _REGISTER_EVENT_HANDLER(HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE,...) \
                                                                          __NL__ \
  namespace kaleidoscope_internal {                                       __NL__ \
                                                                          __NL__ \
   struct EventHandler_##HOOK_NAME {                                      __NL__ \
                                                                          __NL__ \
      static bool shouldAbortOnConsumedEvent() {                          __NL__ \
        return SHOULD_ABORT_ON_CONSUMED_EVENT;                            __NL__ \
      }                                                                   __NL__ \
                                                                          __NL__ \
      template<typename Plugin__, typename... Args__>                     __NL__ \
      static kaleidoscope::EventHandlerResult                             __NL__ \
        call(Plugin__ &plugin, Args__&&... hook_args) {                   __NL__ \
         _VALIDATE_EVENT_HANDLER_SIGNATURE(HOOK_NAME, Plugin__)           __NL__ \
         return plugin.HOOK_NAME(hook_args...);                           __NL__ \
      }                                                                   __NL__ \
    };                                                                    __NL__ \
                                                                          __NL__ \
   } 	                                                                  __NL__ \
                                                                          __NL__ \
   namespace kaleidoscope {                                               __NL__ \
                                                                          __NL__ \
     EventHandlerResult Hooks::HOOK_NAME SIGNATURE {                      __NL__ \
        return kaleidoscope_internal::EventDispatcher::template           __NL__ \
        apply<kaleidoscope_internal::EventHandler_ ## HOOK_NAME>          __NL__ \
             (__VA_ARGS__);                                               __NL__ \
      }                                                                   __NL__ \
                                                                          __NL__ \
   }

// _KALEIDOSCOPE_INIT_PLUGINS builds the loops that execute the plugins'
// implementations of the various event handlers.
//
// Its arguments are a list of references to plugin instances that have been
// instantiated in the global scope.

// EventDispatcher::apply() implements a compile time for-each loop over all
// plugins. The compiler automatically optimizes away calls to any plugin that
// doesn't implement an EventHandler for a given hook.

#define _KALEIDOSCOPE_INIT_PLUGINS(...)                                       __NL__ \
  namespace kaleidoscope_internal {                                           __NL__ \
  struct EventDispatcher {                                                    __NL__ \
                                                                              __NL__ \
    /* Iterate through plugins, calling each one's event handler with         __NL__ \
       the arguments passed to the hook  */                                   __NL__ \
    template<typename EventHandler__, typename... Args__ >                    __NL__ \
    static kaleidoscope::EventHandlerResult apply(Args__&&... hook_args) {    __NL__ \
                                                                              __NL__ \
      kaleidoscope::EventHandlerResult result;                                __NL__ \
      MAP(_INLINE_EVENT_HANDLER_FOR_PLUGIN, __VA_ARGS__)                      __NL__ \
                                                                              __NL__ \
      return result;                                                          __NL__ \
    }                                                                         __NL__ \
  };                                                                          __NL__ \
                                                                              __NL__ \
  }                                                                           __NL__ \
  _REGISTER_EVENT_HANDLER(onSetup,false,())                                   __NL__ \
  _REGISTER_EVENT_HANDLER(beforeEachCycle, false, ())                         __NL__ \
  _REGISTER_EVENT_HANDLER(onKeyswitchEvent, true,                             __NL__ \
               (Key &mappedKey, byte row, byte col, uint8_t keyState),        __NL__ \
               mappedKey, row, col, keyState)                                 __NL__ \
  _REGISTER_EVENT_HANDLER(beforeReportingState,false,())                      __NL__ \
  _REGISTER_EVENT_HANDLER(afterEachCycle,false,())                            __NL__ \

