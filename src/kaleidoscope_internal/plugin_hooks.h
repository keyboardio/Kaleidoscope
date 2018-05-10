/*
 * While this file is mostly pre-processor macros, which are not namespaced, the
 * vast majority of code these macros generate will live under the
 * kaleidoscope_internal namespace. That is why this header is here.
 *
 * The only exception to this is the _KALEIDOSCOPE_INIT_PLUGINS macro, which
 * also places a few symbols under the kaleidoscope namespace. But that code is
 * tightly coupled with the other parts, which are in kaleidoscope_internal. For
 * this reason, this tight coupling, those parts are in this file too.
 *
 * The reason we use an entirely separate namespace is that some plugins have
 * their classes under the kaleidoscope namespace, and the instances of those
 * classes in the global namespace, under the same name. In these cases, the
 * macros herein would resolve to the classes, when we want them to resolve to
 * instances. To avoid this situation, we put the generated code to an entirely
 * different namespace.
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
// The EventHandlers class implements a compile-time loop over all plugins. This
// loop is used to call a non virtual event handler of each plugin.
//
// The advantage of this approach against a solution with virtual event handlers
// in classes derived from kaleidoscope::Plugin is a significant reduction of
// virtual function calls and a reduction of the amount of virtual function
// tables (vtables).
//
// For every handler that is called by the firmware core, only one ordinary
// function call (a static function of class Hooks) is invoked instead of one
// for each plugin and handler. Plugins derived from kaleidoscope::Plugin are
// not (dynamically) polymorphic classes (not featuring virtual methods). Thus,
// classes do not need virtual tables and instances no virtual pointers, which,
// as a consequence, significantly reduces binary size.
//
// Note: A the time of writing this, the kaleidoscope::Plugin is still
//       featuring a virtual begin() method to support legacy implementations
//       of plugins.
//       This means that there is actually still a virtual table generated
//       for each plugin class and a vptr resides in every plugin instance.
//       Those might both vanish when the begin() method would be removed from
//       kaleidoscope::Plugin in future versions of the firmware.
//
// The call to event handlers through kaleidoscope::Hooks and
// kaleidoscope_internal::EventHandlers is templated to allow for compile time
// static polymorphisms (event handlers of plugins not beeing virtual).
//
// This approach is somewhat similar to duck typing in scripting languages like,
// e.g. Python. Only the signature of a method counts and not the fact that the
// class is actually derived from a specific base class. Strictly speaking,
// plugins do not need to be derived from kaleidoscope::Plugin, but only need to
// implement a set of event handlers whose call signature matches.
//
// If a plugin implements an event handler (thereby hiding the equivalent method
// of the plugin base class) the derived plugins's method is called instead of
// the version of the event handler provided by base class kaleidoscope::Plugin.
// This is possible as the event handler is not invoked via a base class ptr or
// reference but via a reference to the actual derived plugin class. Thus the
// compiler can see the actual type of the plugin at the point of the event
// handler's invocation.
//
// EventHandlers::apply() implements a compile time for-each loop over all
// plugins. Under the assumption that only some (few) plugins implement a
// greater number of event handlers and that there is only a limited number of
// plugins used in a sketch, this approach is quite efficient both in terms of
// code size and run-time. This is due to the compiler optimizing away any calls
// to methods that have not been implemented by plugins because the base class
// versions of these are mostly noops. The latter are easy to detect and
// eliminate by the compiler.
//
// Calling the plugins' event handlers is carried out via a call to one of the
// static functions of class Hooks. Those functions cast the related event
// handler on every registered plugin by using the EventHandlers helper class.


// _KALEIDOSCOPE_INIT_PLUGINS executes a MAP over all plugins and uses
// this macro to call the event handler on each one
// TODO rephrase


#define _INLINE_EVENT_HANDLER_FOR_PLUGIN(PLUGIN)                            \
                                                                     __NL__ \
   result = EventHandler__::call(PLUGIN, hook_args...);              __NL__ \
                                                                     __NL__ \
   if (EventHandler__::shouldAbortOnConsumedEvent() &&               __NL__ \
       result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) { __NL__ \
      return result;                                                 __NL__ \
   }                                                                 __NL__


// This defines an auxiliary class 'EventHandler_Foo' for each hook 'Foo'.
// Kaleidoscope::Hooks calls the EventHandlers class, which in turn invokes 
// the event handler method 'Foo' of each registered plugin with a given 
// set of arguments.

#define _REGISTER_EVENT_HANDLER(HOOK, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE,...)                             \
    namespace kaleidoscope_internal {                                     __NL__ \
                                                                          __NL__ \
     struct EventHandler_##HOOK {                                         __NL__ \
                                                                          __NL__ \
      static bool shouldAbortOnConsumedEvent() {                          __NL__ \
        return SHOULD_ABORT_ON_CONSUMED_EVENT;                            __NL__ \
      }                                                                   __NL__ \
                                                                          __NL__ \
      template<typename Plugin__, typename... Args__>                     __NL__ \
      static kaleidoscope::EventHandlerResult call(Plugin__ &plugin,      __NL__ \
                                               Args__&&... hook_args) {   __NL__ \
         _VALIDATE_EVENT_HANDLER_SIGNATURE(HOOK, Plugin__)                __NL__ \
                                                                          __NL__ \
         return plugin.HOOK(hook_args...);                                __NL__ \
      }                                                                   __NL__ \
    };                                                                    __NL__ \
   } 	                                                                  __NL__ \
                                                                          __NL__ \
   namespace kaleidoscope {                                               __NL__ \
     EventHandlerResult Hooks::HOOK SIGNATURE {                           __NL__ \
        return kaleidoscope_internal::EventHandlers::template             __NL__ \
        apply<kaleidoscope_internal::EventHandler_ ## HOOK>(__VA_ARGS__); __NL__ \
      }                                                                   __NL__ \
   }

// _KALEIDOSCOPE_INIT_PLUGINS builds the loops that execute the plugins' 
// implementations of the various event handlers.
//
// Its arguments are a list of references to plugin instances that have been
// instantiated in the global scope.


#define _KALEIDOSCOPE_INIT_PLUGINS(...)                                       \
   namespace kaleidoscope_internal {                                   __NL__ \
   struct EventHandlers {                                                     __NL__ \
      /* Call the event handler on the plugin with the handler's arguments */ __NL__ \
      template<typename EventHandler__, typename... Args__ >                  __NL__ \
      static kaleidoscope::EventHandlerResult apply(Args__&&... hook_args) {  __NL__ \
         kaleidoscope::EventHandlerResult result;                             __NL__ \
                                                                              __NL__ \
         MAP(_INLINE_EVENT_HANDLER_FOR_PLUGIN, __VA_ARGS__)                   __NL__ \
                                                                              __NL__ \
         return result;                                                       __NL__ \
      }                                                                       __NL__ \
   };                                                                         __NL__ \
   }                                                                          __NL__ \
                                                                              __NL__ \
   _REGISTER_EVENT_HANDLER(onSetup,false,())                                    __NL__ \
   _REGISTER_EVENT_HANDLER(beforeEachCycle, false, ())                          __NL__ \
   _REGISTER_EVENT_HANDLER(onKeyswitchEvent, true,                              __NL__ \
                (Key &mappedKey, byte row, byte col, uint8_t keyState),       __NL__ \
                mappedKey, row, col, keyState)                                __NL__ \
   _REGISTER_EVENT_HANDLER(beforeReportingState,false,())                       __NL__ \
   _REGISTER_EVENT_HANDLER(afterEachCycle,false,())                             __NL__ \

