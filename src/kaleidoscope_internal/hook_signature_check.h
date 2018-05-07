#pragma once

#include "macro_helpers.h"
#include "plugin.h"

// We use non-virtual methods for hooks. This reduces PROGMEM consumption,
// runtime call overhead, and allows the compiler to a better job
// removing dead card.
//
// The downside is that hook methods in derived plugins are hidden, 
// rather than overridden. Because only virtual methods can be overridden,
// we can't apply C++'s `override` keyword to hook methods.
//
// To make it easier to debug issues with plugins' hook method signatures,
// the _VALIDATE_HOOK_METHOD_SIGNATURE macro compares a plugin's hook method 
// signatures with those of the kaleidoscope::Plugin baseclass. If any 
// differences are detected, it outputs a compile-time error message.
// 
//
// Ideally, we'd be able to use this (much simpler) code.
//
//    decltype(&::kaleidoscope::Plugin::fooHookMethod)
//              == decltype(&childPlugin::fooHookMethod)
//
// Unfortunately, this is not possible with C++-11, as it does not allow
// comparing function-types for equality. As a workaround, we can use
// the trait class HookSignaturesMatch to perform the comparison.
//
// It defines the bool constant 'value' as true if both signatures match.
// It checks the return value, argument types and const specification.
// (As of this writing, we don't have any const hook method.)

template<typename T1, typename T2>
struct HookSignaturesMatch {
  static constexpr bool value = false;
};

// R: The return value, 
// T1: Type of the first class (plugin),
// T2: Type of the second class (plugin), 
// HookArgs: Variadic types of plugin hook arguments.

template<typename R, typename T1, typename T2, typename... HookArgs>
struct HookSignaturesMatch<R(T1::*)(HookArgs...), R(T2::*)(HookArgs...)> {
  static constexpr bool value = true;
};

// Equivalent to allow for const-hooks, e.g. bool getFooHook() const

template<typename R, typename T1, typename T2, typename... HookArgs> struct
HookSignaturesMatch<R(T1::*)(HookArgs...) const, R(T2::*)(HookArgs...) const> {
  static constexpr bool value = true;
};

// This template is instantiated when something goes wrong. 
// Because it does not define a constant 'value', it triggers a compiler error.

template<typename Plugin__, bool result> struct
  ___________Culprit_Plugin___________ { };

// This specialization is instantiated when everything is ok.
template<typename Plugin__> struct 
  ___________Culprit_Plugin___________ <Plugin__, true> {
  static constexpr bool value = true;
};

// If the pointer types are the same, the signatures match, causing 
// the first or second specialization to be instantiated. This makes
// the causes the compile time constant `value` to be defined as `true`.
// Otherwise, the unspecialized version of the template class is instantiated
// and `value` is defined as `false`

#define _VALIDATE_HOOK_METHOD_SIGNATURE(HOOK_METHOD, PLUGIN)                   \
{                                                                       __NL__ \
  /* Check if the signatures match. If not, the plugin has implemented  __NL__ \
  * a method with a hook's name, but a different signature.             __NL__ \
  */                                                                    __NL__ \
  typedef HookSignaturesMatch<                                          __NL__ \
                     decltype(&::kaleidoscope::Plugin::HOOK_METHOD),    __NL__ \
                     decltype(&PLUGIN::HOOK_METHOD)                     __NL__ \
                > Check;                                                __NL__ \
                                                                        __NL__ \
  static_assert(Check::value,                                           __NL__ \
    VERBOSE_STATIC_ASSERT_HEADER                                        __NL__ \
   "\nOne of your plugins implemented the hook \"" #HOOK_METHOD "\","   __NL__ \
   "\nbut its signature didn't match the base class."                   __NL__ \
   "\n"                                                                 __NL__ \
   "\nThe plugin with this issue will be marked in the compiler"        __NL__ \
   "\noutput with the string:"                                          __NL__ \
   "\n"                                                                 __NL__ \
   "\n      ___________Culprit_Plugin___________."                      __NL__ \
   "\n"                                                                 __NL__ \
   "\nYou should compare the hook methods implemented in this plugin"   __NL__ \
   "\nto those in \"kaleidoscope::Plugin\"."                            __NL__ \
   "\n"                                                                 __NL__ \
   "\nAll of the hook method's argument types, return values and"       __NL__ \
   "\nconst qualifiers need to match."                                  __NL__ \
   "\n"                                                                 __NL__ \
    VERBOSE_STATIC_ASSERT_FOOTER                                        __NL__ \
  );                                                                    __NL__ \
                                                                        __NL__ \
  /* The following construct is necessary enable reporting of the       __NL__ \
   * type of a plugin that implements a hook with an incorrect          __NL__ \
   * because it's not possible to include any non-literal string        __NL__ \
   * constant in a static_assert error message.                         __NL__ \
   */                                                                   __NL__ \
  constexpr bool dummy = ___________Culprit_Plugin___________           __NL__ \
       <PLUGIN, Check::value>::value;                                   __NL__ \
}
