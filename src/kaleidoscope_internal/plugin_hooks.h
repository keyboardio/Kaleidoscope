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
#include "hook_signature_check.h"

namespace kaleidoscope_internal {

// A note to maintainers: How to add additional hooks
//
// 1) Add an appropriate non-virtual (important!) method
//       to class kaleidoscope::Plugin.
// 2) Add a static method with the same name and call signature
//       to class kaleidoscope::Hooks.
// 3) Add an implemenation of the new static method of kaleidoscope::Hooks
//       introduced in 2) to the end of macro KALEIDOSCOPE_INIT_PLUGINS(...)
//       and a matching weak implementation to kaleidoscope/hooks.cpp

// Some words about the design of hook rooting:
//
// The HookPoint class implements a compile-time loop over all plugins. This
// loop is used to call a non virtual hook method of each plugin.
//
// The advantage of this approach against a solution with virtual
// hook methods in classes derived from kaleidoscope::Plugin is a significant
// reduction of virtual function calls and a reduction of the amount of
// virtual function tables (vtables).
//
// For every hook that is called by the firmware core, only one
// ordinary function call (a static function of class Hooks) is invoked instead of
// one for each plugin and hook. Plugins derived
// from kaleidoscope::Plugin are not (dynamically) polymorphic classes
// (not featuring virtual methods). Thus, classes do not need virtual tables
// and instances no virtual pointers, which, as a consequence,
// significantly reduces binary size.
//
// Note: A the time of writing this, the kaleidoscope::Plugin is still
//       featuring a virtual begin() method to support legacy implementations
//       of plugins.
//       This means that there is actually still a virtual table generated
//       for each plugin class and a vptr resides in every plugin instance.
//       Those might both vanish when the begin() method would be removed from
//       kaleidoscope::Plugin in future versions of the firmware.
//
// The call to hook methods through kaleidoscope::Hooks and
// kaleidoscope_internal::HookPoint is templated to allow for compile time
// static polymorphisms (hook methods of plugins not beeing virtual).
//
// This approach is somewhat similar to duck typing in scripting languages like,
// e.g. Python. Only the signature of a method counts and not the fact that the
// class is actually derived from a specific base class. Strictly speaking,
// plugins do not need to be derived from kaleidoscope::Plugin, but only need to
// implement a set of hooks whose call signature matches.
//
// If a plugin implements a hook method (thereby hiding the equivalent method
// of the plugin base class) the derived plugins's method is called instead
// of the version of the hook method provided by base class kaleidoscope::Plugin.
// This is possible as the hook method is not invoked via a base class ptr
// or reference but via a reference to the actual derived plugin class.
// Thus the compiler can see the actual type of the plugin at the point of
// the hook method's invocation.
//
// HookPoint::apply() implements a compile time for-each loop over all plugins.
// Under the assumption that only some (few) plugins implement a greater number
// of hook methods and that there is only a limited number of plugins used in a
// sketch, this approach is quite efficient both in terms of code size and
// run-time. This is due to the compiler optimizing away any calls to hooks that
// have not been implemented by plugins because the base class versions of these
// hooks are mostly noops. The latter are easy to detect and eliminate by the
// compiler.
//
// Calling the plugins' hook methods is carried out via a call to one of the
// static functions of class Hooks. Those functions cast the related hook method
// on every registered plugin by using the HookPoint helper class.

// The _EVENT_HANDLER macro defines an auxiliary classes EventHandler_... that
// invoke a specific plugin hook method with a provided set of method arguments.
// The EventHandler_... classes are meant to be used by HookPoint's apply
// function called in the static functions of kaleidoscope::Hooks to forward the
// call to the hook methods of all registered plugins.
#define _EVENT_HANDLER(EVENT_HANDLER, SHOULD_ABORT_ON_CONSUMED_EVENT)            \
   struct EventHandler_##EVENT_HANDLER {                                  __NL__ \
                                                                          __NL__ \
      static bool shouldAbortOnConsumedEvent() {                          __NL__ \
        return SHOULD_ABORT_ON_CONSUMED_EVENT;                            __NL__ \
      }                                                                   __NL__ \
                                                                          __NL__ \
      template<typename Plugin__, typename... Args__>                     __NL__ \
      static kaleidoscope::EventHandlerResult call(Plugin__ &plugin,          __NL__ \
                                               Args__&&... hook_args) {   __NL__ \
         _VALIDATE_HOOK_METHOD_SIGNATURE(EVENT_HANDLER, Plugin__)         __NL__ \
                                                                          __NL__ \
         return plugin.EVENT_HANDLER(hook_args...);                       __NL__ \
      }                                                                   __NL__ \
};

_EVENT_HANDLER(onSetup, false)
_EVENT_HANDLER(beforeEachCycle, false)
_EVENT_HANDLER(onKeyswitchEvent, true)
_EVENT_HANDLER(beforeReportingState, false)
_EVENT_HANDLER(afterEachCycle, false)

// The following _CALL macros are used by DEFINE_HOOKPOINT in
// conjunction with the MAP macro to call the hook method in each listed plugin.

#define _CALL_HOOK_FOR_PLUGIN(PLUGIN)                                       \
   result = EventHandler__::call(PLUGIN, hook_args...);              __NL__ \
                                                                     __NL__ \
   if (EventHandler__::shouldAbortOnConsumedEvent() &&               __NL__ \
       result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {     __NL__ \
      return result;                                                 __NL__ \
   }                                                                 __NL__

#define _DEFINE_HOOKPOINT(CLASS_NAME, ...)                            \
   struct CLASS_NAME                                                      __NL__ \
   {                                                                      __NL__ \
      /* Call the hook method on the plugin with the hook's arguments */  __NL__ \
      template<typename EventHandler__, typename... Args__ >              __NL__ \
      static kaleidoscope::EventHandlerResult apply(Args__&&... hook_args) {  __NL__ \
         kaleidoscope::EventHandlerResult result;                             __NL__ \
                                                                          __NL__ \
         MAP(_CALL_HOOK_FOR_PLUGIN, __VA_ARGS__)                          __NL__ \
                                                                          __NL__ \
         return result;                                                   __NL__ \
      }                                                                   __NL__ \
   };

#define _DEFINE_HOOKPOINT_METHOD(HOOK,SIGNATURE,...) \
  EventHandlerResult Hooks::HOOK SIGNATURE {                               __NL__ \
     return kaleidoscope_internal::HookPoint::template                 __NL__ \
     apply<kaleidoscope_internal::EventHandler_ ## HOOK>(__VA_ARGS__); __NL__ \
   }                                                                   __NL__

/* KALEIDOSCOPE_INIT_PLUGINS should only be invoked in global namespace of the
   firmware sketch. It turns around and calls _KALEIDOSCOPE_INIT_PLUGINS here,
   which actually builds the loops that execute the plugins' implementations of
   the various hooks.

   Its arguments are a list of references to plugin instances that have been
   instantiated in the global scope. */
#define _KALEIDOSCOPE_INIT_PLUGINS(...)                                       \
   namespace kaleidoscope_internal {                                   __NL__ \
                                                                       __NL__ \
   _DEFINE_HOOKPOINT(HookPoint, __VA_ARGS__)                           __NL__ \
                                                                       __NL__ \
   }                                                                   __NL__ \
                                                                       __NL__ \
   namespace kaleidoscope {                                            __NL__ \
   _DEFINE_HOOKPOINT_METHOD(onSetup,())                                __NL__ \
   _DEFINE_HOOKPOINT_METHOD(beforeEachCycle, ())                       __NL__ \
   _DEFINE_HOOKPOINT_METHOD(onKeyswitchEvent,                          __NL__ \
                (Key &mappedKey, byte row, byte col, uint8_t keyState),__NL__ \
                mappedKey, row, col, keyState)                         __NL__ \
   _DEFINE_HOOKPOINT_METHOD(beforeReportingState,())                   __NL__ \
   _DEFINE_HOOKPOINT_METHOD(afterEachCycle,())                         __NL__ \
   }
}
