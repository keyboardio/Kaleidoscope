#pragma once

#include "macro_functions.h"
#include "plugin.h"
#include "hook_signature_check.h"

// A note to maintainers: How to add additional hooks
//
// 1) Add an appropriate non-virtual (important!) method
//       to class KaleidoscopePlugin
// 2) Add a virtual (important!) method with the same name and call signature
//       to the abstract base class PluginHookAdapter__
// 3) Add a hook specification to template class PluginHookAdapter
//       via the HOOK_TASK macro
// 4) Add an implemenation of the hook to template class PluginHookAdapter
//       that references the name given in HOOK_TASK

// Some words about the design of the plugin interface
//
// The HookLoop class implements a compile-time loop over all plugins.
// This loop is used to call a non virtual hook method of each plugin.
//
// The advantage of this approach against a solution with virtual
// hook methods in classes derived from KaleidoscopePlugin is a significant
// reduction of virtual function calls and a reduction of the amount of
// virtual function tables (vtables).
//
// For every hook that is called by the firmware core, only one virtual
// method (of PluginHookAdapter) is invoked instead of
// one for each plugin and hook. As with this approach plugins derived
// from KaleidoscopePlugin are not (dynamically) polymorphic classes
// (not featuring virtual methods). Thus, classes do not need virtual tables
// and instances no virtual pointers, which, as a consequence,
// significantly reduces binary size.
//
// Note: A the time of writing this, the KaleidoscopePlugin is still
//       featuring a virtual begin() method to support legacy implementations
//       of plugins.
//       This means that there is actually still a virtual table generated
//       for each plugin class and a vptr resides in every plugin instance.
//       Those might both vanish when the begin() method would be removed from
//       KaleidoscopePlugin in future versions of the firmware.
//
// The call to hook methods through PluginHookAdapter and HookLoop
// is templated to allow for compile time static polymorphisms
// (hook methods of plugins not beeing virtual).
//
// ***For non template experts:
//       This is somewhat similar to duck typing in scripting languages
//       as only the signature of a method counts and not the fact that the class
//       is actually derived from a base class. Thus, strictly speaking,
//       plugins do not need to be derived from KaleidoscopePlugin, but
//       only need to an implement equivalent set of hooks.
//
// If a plugin implements a method (thereby hiding the equivalent method
// of the plugin base class) the derived plugins's method is called instead
// of the version of the hook provided by the base class KaleidoscopePlugin.
// This is possible as the hook method is not invoked via a base class ptr
// or reference but via a reference to the actual derived plugin class.
//
// HookLoop::apply() implements a compile time for-each loop
// over all plugins. Under the assumption that only some (few) plugins
// implement many hook methods and that there is only a limited number
// of plugins used in a sketch, this approach is quite efficient both in terms
// of code size and run-time. This is due to the compiler
// optimizing away any calls to hooks that have not been
// implemented by plugins because the base class versions of these hooks are
// mostly noops which are easy to detect and eliminate by the compiler.
//
// Calling the plugins hook method is finally carried out via the Hook__ template
// argument which forwards the call to the hook method to the actual plugin.
//
// Some hooks such as e.g. the eventHandlerHook require a decision about
// whether to continue calling other hook methods after them, or,
// if required, to abort the loop. This decision is implemented through a
// ContinuationPredicate__, a helper class
// whose eval-method generates a boolean return value that is based on the
// hooks' return values. If the predicate's result is true, the loop
// continues or is aborted otherwise. The eventHandlerHook method's
// return value, e.g. signals by itself whether it wants to allow other
// eventHandlerHooks to be called after it.
// By means of this approach the decision is for most hooks evaluated at
// compile-time, which e.g. cases the decision to be optimized out
// for all those hook methods that have a void return value.


// KALEIDOSCOPE_INIT_PLUGINS is meant to be invoked at global scope of
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
#define KALEIDOSCOPE_INIT_PLUGINS(...)                                         \
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
__NL__                              -> typename Hook__::ReturnValue {          \
__NL__                                                                         \
__NL__         typename Hook__::ReturnValue hook_return_val;                   \
__NL__                                                                         \
__NL__         MAP(INVOKE_HOOK_FOR_PLUGIN, __VA_ARGS__)                        \
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
__NL__      static auto apply() -> typename Hook__::ReturnValue {              \
__NL__         MAP(INVOKE_EMPTY_ARGS_HOOK_FOR_PLUGIN, __VA_ARGS__)             \
__NL__      }                                                                  \
__NL__   };                                                                    \
__NL__                                                                         \
__NL__   PluginHookAdapter<HookLoop> hookLoop;

// This macro is supposed to be called at the end of the begin() method
// of the firmware sketch to connect the plugin hooks.
//
// Unfortunately, the c++ standard (until c++17) does not allow for
// local class templates, neither for classes that feature template functions.
// Thus we have to separate initialization and connection of plugins.
// Otherwise, everything could be called from within the begin() method
// of the firmware sketch.
//
#define KALEIDOSCOPE_CONNECT_PLUGINS                                           \
   Kaleidoscope.connectPlugins(&hookLoop);

// The following invoke macros are meant to be used with
// KALEIDOSCOPE_CONNECT_PLUGINS in conjunction with the MAP macro
// that casts a specific operation on every member of
// a variadic macro argument list (if present).
//
#define INVOKE_HOOK_FOR_PLUGIN(PLUGIN)                                         \
__NN__                                                                         \
__NN__   hook_return_val = Hook__::invoke(PLUGIN, hook_args...);               \
__NL__                                                                         \
__NL__   if (!ContinuationPredicate__::eval(hook_return_val)) {                \
__NL__      return hook_return_val;                                            \
__NL__   }

#define INVOKE_EMPTY_ARGS_HOOK_FOR_PLUGIN(PLUGIN)                              \
   Hook__::invoke(PLUGIN);

class EventKey;

// This hook adapter base class defines the main interface that
// is used by the Kaleidoscope class and in all other places
// where hooks are called.
//
class PluginHookAdapter__ {
 public:

  virtual void init() {}

  virtual bool eventHandlerHook(Key &mappedKey, const EventKey &eventKey) {
    return true;
  }

  virtual void preReportHook() {}
  virtual void postReportHook() {}
};



// The HOOK_TASK macro defines an auxiliary Hook class (HOOK) that invokes
// a plugin hook method with a provided set of method arguments. The
// HOOK class is meant to be passed to the PluginLoop in the PluginHookAdapter
// to forward the call to the hook methods of plugins.
//
#define HOOK_TASK(HOOK, RET_VAL, HOOK_METHOD)                                  \
__NN__                                                                         \
__NN__   struct HOOK {                                                         \
__NL__                                                                         \
__NL__      typedef RET_VAL ReturnValue;                                       \
__NL__                                                                         \
__NL__      template<typename Plugin__, typename... Args__>                    \
__NL__      static RET_VAL invoke(Plugin__ &plugin, Args__&&... hook_args)     \
__NL__      {                                                                  \
__NL__         MATCH_HOOK_TYPE(HOOK_METHOD, Plugin__)                          \
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

// This is the implementation of the PluginHookAdapter__ interface.
// It is templated to allow it to adapt itself to the list of plugins
// that is used in the sketch. This information is known at compile time
// but differs between sketches as part of the firmware configuration.
//
// The PluginHookAdapter operates on a PluginLoop__, an
// interface class that casts Hook classes on all plugins, therby
// forwaring the actual hook calls. Hook overrides may
// specify ContinuationPredicate__ classes that decide
// if further hook methods of other plugins are allowed to be called based
// on the return value of the most recent call to a hook, e.g. a boolean value
// (see the eventHandlerHook for an example).
//
template<typename PluginLoop__>
class PluginHookAdapter : public PluginHookAdapter__ {
 public:

  HOOK_TASK(InitTask, void, init)
  void init() final {
    PluginLoop__::template apply<InitTask>();
  }

  HOOK_TASK(EventHandlerHookTask, bool, eventHandlerHook)
  bool eventHandlerHook(Key &mappedKey, const EventKey &eventKey) final {
    return PluginLoop__::template apply<EventHandlerHookTask, ContinueIfHookReturnsTrue>
    (mappedKey, eventKey);
  }

  HOOK_TASK(preReportHookTask, void, preReportHook)
  void preReportHook() final {
    PluginLoop__::template apply<preReportHookTask>();
  }

  HOOK_TASK(postReportHookTask, void, postReportHook)
  void postReportHook() final {
    PluginLoop__::template apply<postReportHookTask>();
  }
};
