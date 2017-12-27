#pragma once

#include "macro_functions.h"
#include "plugin.h"
#include "details/hooks.h"
#include "hook_signature_check.h"

namespace kaleidoscope {

// A note to maintainers: How to add additional hooks
//
// 1) Add an appropriate non-virtual (important!) method
//       to class kaleidoscope::Plugin.
// 2) Add a static method with the same name and call signature
//       to class kaleidoscope::Hooks.
// 3) Add an implemenation of the new static method of kaleidoscope::Hooks
//       introduced in 2) to the end of macro KALEIDOSCOPE_INIT_PLUGINS(...)
//       and a matching weak implementation to details/hooks.cpp

// Some words about the design of hook rooting:
//
// The HookLoop class implements a compile-time loop over all plugins.
// This loop is used to call a non virtual hook method of each plugin.
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
// The call to hook methods through kaleidoscope::Hooks and kaleidoscope::HookLoop
// is templated to allow for compile time static polymorphisms
// (hook methods of plugins not beeing virtual).
//
// ***For non template experts:
//       This approach is somewhat similar to duck typing in scripting languages
//       like, e.g. Python. Only the signature of a method counts and not
//       the fact that the class
//       is actually derived from a specific base class. Strictly speaking,
//       plugins do not need to be derived from kaleidoscope::Plugin, but
//       only need to implement a set of hooks whose call signature matches.
//
// If a plugin implements a hook method (thereby hiding the equivalent method
// of the plugin base class) the derived plugins's method is called instead
// of the version of the hook method provided by base class kaleidoscope::Plugin.
// This is possible as the hook method is not invoked via a base class ptr
// or reference but via a reference to the actual derived plugin class.
// Thus the compiler can see the actual type of the plugin at the point of
// the hook method's invocation.
//
// HookLoop::apply() implements a compile time for-each loop
// over all plugins. Under the assumption that only some (few) plugins
// implement a greater number of hook methods and that there is only a limited number
// of plugins used in a sketch, this approach is quite efficient both in terms
// of code size and run-time. This is due to the compiler
// optimizing away any calls to hooks that have not been
// implemented by plugins because the base class versions of these hooks are
// mostly noops. The latter are easy to detect and eliminate by the compiler.
//
// Calling the plugins' hook methods is carried out via a call to
// one of the static functions of class Hooks. Those functions cast the
// related hook method on every registered plugin by using the HookLoop helper
// class.
//
// Some hooks such as e.g. the eventHandlerHook require a decision about
// whether to continue calling other hook methods after them, or,
// if required, to abort the loop over the registered plugins.
// This decision is implemented through predicated ContinuationPredicate__,
// a helper class
// whose eval-method generates a boolean return value that is computed
// based on the hooks' return values. If the predicate's result is true, the loop
// continues and is aborted otherwise. The eventHandlerHook method's
// return value, e.g., signals by itself whether it wants to allow other
// eventHandlerHooks to be called after it.
// By means of this approach for most hooks the decision is evaluated at
// compile-time, which e.g. causes the decision to be optimized out
// for all those hook methods that have a void return value.

// ReturnTypeTraits is an auxiliary traits class that determines the
// return type of a hook member function.
//
template<typename T>
struct ReturnTypeTraits {};

template<typename R, typename T, typename... HookArgs>
struct ReturnTypeTraits<R(T::*)(HookArgs...)> {
  typedef R Type;
};

// The _HOOK_TASK macro defines an auxiliary classes HookTask_... that invoke
// a specific plugin hook method with a provided set of method arguments. The
// HookTask_... classes are meant to be used by HookLoop's apply function
// called in the static functions of kaleidoscope::Hooks to forward the call to
// the hook methods of all registered plugins.
//
#define _HOOK_TASK(HOOK_METHOD)                                                \
__NN__                                                                         \
__NN__   struct HookTask_##HOOK_METHOD {                                       \
__NL__                                                                         \
__NN__      /* Use a traits class to determine the return value type of the    \
__NN__         hook method.                                                    \
__NN__       */                                                                \
__NL__      typedef typename ReturnTypeTraits<                                 \
__NL__                 decltype(&kaleidoscope::Plugin::HOOK_METHOD)            \
__NL__              >::Type  ReturnType;                                       \
__NL__                                                                         \
__NL__      template<typename Plugin__, typename... Args__>                    \
__NL__      static ReturnType invoke(Plugin__ &plugin,                         \
__NL__                                    Args__&&... hook_args)               \
__NL__      {                                                                  \
__NN__         /* Assert that hook methods in derived plugins have an          \
__NN__            call signature.                                              \
__NN__          */                                                             \
__NL__         _MATCH_HOOK_TYPE(HOOK_METHOD, Plugin__)                         \
__NN__                                                                         \
__NL__         return plugin.HOOK_METHOD(hook_args...);                        \
__NL__      }                                                                  \
__NL__   };

// A predicate class that decides on hook-plugin-loop continuation.
//
struct ContinueIfHookReturnsTrue {
  static bool eval(bool val) {
    return val;
  }
};

// This macro implements the static hook rooting functions of
// class kaleidoscope::Hooks based on the registered plugins.
// It must be invoked in global namespace as it adds namespace kaleidoscope
// to the Hooks' function implementations.
//
#define _HOOKS_STATIC_METHODS_IMPLEMENTATION                                   \
__NN__                                                                         \
__NN__   namespace kaleidoscope {                                              \
__NN__                                                                         \
__NL__   _HOOK_TASK(init) /* Defines HookTask_init */                          \
__NL__   void Hooks::init() {                                                  \
__NL__     HookLoop::template apply<HookTask_init>();                          \
__NL__   }                                                                     \
__NL__                                                                         \
__NL__   _HOOK_TASK(eventHandlerHook) /* Defines HookTask_eventHandlerHook */  \
__NL__   bool Hooks::eventHandlerHook(Key &mappedKey,                          \
__NL__                                const EventKey &eventKey) {              \
__NL__     return HookLoop                                                     \
__NL__               ::template apply<HookTask_eventHandlerHook,               \
__NL__                                ContinueIfHookReturnsTrue>               \
__NL__                             (mappedKey, eventKey);                      \
__NL__   }                                                                     \
__NL__                                                                         \
__NL__   _HOOK_TASK(preReportHook) /* Defines HookTask_preReportHook */        \
__NL__   void Hooks::preReportHook() {                                         \
__NL__     HookLoop::template apply<HookTask_preReportHook>();                 \
__NL__   }                                                                     \
__NL__                                                                         \
__NL__   _HOOK_TASK(postReportHook) /* Defines HookTask_postReportHook */      \
__NL__   void Hooks::postReportHook() {                                        \
__NL__     HookLoop::template apply<HookTask_postReportHook>();                \
__NL__   }                                                                     \
__NL__                                                                         \
__NL__   } /* namespace kaleidoscope */

// The following invoke macros are meant to be used with
// KALEIDOSCOPE_CONNECT_PLUGINS in conjunction with the MAP macro
// (from header "macro_functions.h") that casts a specific operation on
// every member of a variadic macro argument list (if present).
//
#define _INVOKE_HOOK_FOR_PLUGIN(PLUGIN)                                        \
__NN__                                                                         \
__NN__   hook_return_val = Hook__::invoke(PLUGIN, hook_args...);               \
__NL__                                                                         \
__NL__   if (!ContinuationPredicate__::eval(hook_return_val)) {                \
__NL__      return hook_return_val;                                            \
__NL__   }

#define INVOKE_EMPTY_ARGS_HOOK_FOR_PLUGIN(PLUGIN)                              \
   Hook__::invoke(PLUGIN);

// KALEIDOSCOPE_INIT_PLUGINS is meant to be invoked in global namespace of
// the firmware sketch.
//
// Arguments: A list of references to plugin instances that have been
//       instanciated at global scope.
//
// A note concerning possible future optimizations:
//    In C++17 the loop over all plugins, and calling a hook method
//    with a given list of arguments can be implemented much more elegant
//    without using preprocessor macros.
//    The alternative solution would consist in a combination
//    of std::forward_as_tuple and std::apply to call a function
//    (the hook method) for every member of a tuple (containing rvalue
//    references to all plugins). Thus, both the list of plugins and
//    and the hook method call arguments (both variadic) could be wrapped
//    in rvalue-reference-tuples and be passed to some kind
//    of tuple-for-each algorithm.
//
// Note: KALEIDOSCOPE_INIT_PLUGINS(...) must only be invoked in global namespace.
//
#define _KALEIDOSCOPE_INIT_PLUGINS(...)                                        \
__NN__                                                                         \
__NN__   namespace kaleidoscope {                                              \
__NN__                                                                         \
__NN__   struct HookLoop                                                       \
__NL__   {                                                                     \
__NL__      template<typename Hook__, /* Invokes the hook method               \
__NN__                                   on the plugin */                      \
__NL__               typename ContinuationPredicate__, /* Decides whether      \
__NN__                                   to continue with further hooks */     \
__NL__               typename... Args__ /* The hook method call arguments */   \
__NL__      >                                                                  \
__NL__      /* The Hook__ class defines the return value of the hook method    \
__NL__         as a nested typedef.                                            \
__NL__         To determine the actual return type based on Hook__, we have    \
__NL__         to rely on the trailing-return-type syntax. */                  \
__NL__      static auto apply(Args__&&... hook_args)                           \
__NL__                              -> typename Hook__::ReturnType {           \
__NL__                                                                         \
__NL__         typename Hook__::ReturnType hook_return_val;                    \
__NL__                                                                         \
__NL__         MAP(_INVOKE_HOOK_FOR_PLUGIN, __VA_ARGS__)                       \
__NL__                                                                         \
__NL__         return hook_return_val;                                         \
__NL__      }                                                                  \
__NL__                                                                         \
__NL__      /* An overloaded empty arguments version of apply to support       \
__NL__         hooks with void arguments */                                    \
__NL__      template<typename Hook__,                                          \
__NL__                     /* Invokes the hook method on the plugin */         \
__NL__               typename... Args__ /* The hook method call arguments */   \
__NL__      >                                                                  \
__NL__      static auto apply() -> typename Hook__::ReturnType {               \
__NL__         MAP(INVOKE_EMPTY_ARGS_HOOK_FOR_PLUGIN, __VA_ARGS__)             \
__NL__      }                                                                  \
__NL__   };                                                                    \
__NL__                                                                         \
__NL__   } /* namespace kaleidoscope */                                        \
__NL__                                                                         \
__NN__   _HOOKS_STATIC_METHODS_IMPLEMENTATION

} // namespace kaleidoscope
