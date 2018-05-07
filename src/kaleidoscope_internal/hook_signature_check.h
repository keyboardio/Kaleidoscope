#pragma once

#include "macro_helpers.h"
#include "plugin.h"

// We use non-virtual methods for hooks. This reduces PROGMEM consumption and
// runtime call overhead and allows for dead code elimination.
//
// This is not without additional costs as we
// have to deal with the problem that hook methods in derived plugins are
// hidden, rather than overridden (only virtual methods can be overridden).
// This means that c++'s override keyword cannot be applied for our hooks.
//
// To prevent an accidental creation of a hook method with a wrong signature,
// we compare the base class hook method's signature
// with the signature of the hook method being actually called. If both types
// differ, something went wrong and we output a verbose message that informs
// the user about how the error can be fixed.
//
// What we would like to check is the following.
//
//    decltype(&::kaleidoscope::Plugin::fooHookMethod)
//              == decltype(&childPlugin::fooHookMethod)
//
// Unfortunately, this is not possible with c++(-11) as it does not allow
// to compare two (function-)types for equality.
//
// Because of this, the comparison is done based on the
// traits class HookSignaturesMatch.
// It defines the bool constant 'value' as true if both signatures
// considering return value, call argument types and const specification
// equal. We also allow for future const-hook methods here.

template<typename T1, typename T2>
struct HookSignaturesMatch {
  static constexpr bool value = false;
};

// R: The return value, T1: Type of the first class (plugin),
// T2: Type of the second class (plugin), HookArgs: Variadic types of
// plugin hook arguments.

template<typename R, typename T1, typename T2, typename... HookArgs>
struct HookSignaturesMatch<R(T1::*)(HookArgs...), R(T2::*)(HookArgs...)> {
  static constexpr bool value = true;
};

// Equivalent to allow for const-hooks, e.g. bool getFooHook() const

template<typename R, typename T1, typename T2, typename... HookArgs>
struct HookSignaturesMatch<R(T1::*)(HookArgs...) const, R(T2::*)(HookArgs...) const> {
  static constexpr bool value = true;
};

// This template is instantiated when something goes wrong. It causes a
// compiler error as it does not define a constant 'value'.

template<typename Plugin__, bool result>
struct ___________Culprit_Plugin___________ {
};

// This specialization is instantiated when everything is ok.

template<typename Plugin__>
struct ___________Culprit_Plugin___________
  <Plugin__, true> {
  static constexpr bool value = true;
};

// The above specialized HookSignaturesMatch plugin class is instantiated
// based on two types of pointers to the plugin base class hook method and
// the derived method. If both pointer types and thus the signatures equal,
// the first or second specialization is instantiated,
// leading to its compile time constant 'value' being defined as true.
// Else the unspecialized version of the template class is instantiated,
// causing 'value' to be defined as false.

#define _VALIDATE_HOOK_METHOD_SIGNATURE(HOOK_METHOD, PLUGIN)                   \
{                                                                       __NL__ \
  /* Check it the two signatures equal. If not, the                     __NL__ \
  * user has implemented a method with the same name but a              __NL__ \
  * different (wrong) signature.                                        __NL__ \
  */                                                                    __NL__ \
  typedef HookSignaturesMatch<                                          __NL__ \
                     decltype(&::kaleidoscope::Plugin::HOOK_METHOD),    __NL__ \
                     decltype(&PLUGIN::HOOK_METHOD)                     __NL__ \
                > Check;                                                __NL__ \
                                                                        __NL__ \
  static_assert(Check::value,                                           __NL__ \
    VERBOSE_STATIC_ASSERT_HEADER                                        __NL__ \
   "\nOne of your plugins implemented a hook \"" #HOOK_METHOD "\""      __NL__ \
   "\nwhose signature differs from the base class' hook signature."     __NL__ \
   "\n"                                                                 __NL__ \
   "\nPlease check the compiler messages above or below for an hint"    __NL__ \
   "\nabout which plugin is the culprit. It will be marked by the"      __NL__ \
   "\nstring"                                                           __NL__ \
   "\n"                                                                 __NL__ \
   "\n      ___________Culprit_Plugin___________."                      __NL__ \
   "\n"                                                                 __NL__ \
   "\nThen check all the hook methods implemented by this plugin"       __NL__ \
   "\nand compare them to the respective hook methods of"               __NL__ \
   "\nbase class \"kaleidoscope::Plugin\"."                             __NL__ \
   "\n"                                                                 __NL__ \
   "\nNote: Two method signatures equal if all their argument"          __NL__ \
   "\n      types, the methods' const qualifiers and their"             __NL__ \
   "\n      return values equal."                                       __NL__ \
   "\n"                                                                 __NL__ \
    VERBOSE_STATIC_ASSERT_FOOTER                                        __NL__ \
  );                                                                    __NL__ \
                                                                        __NL__ \
  /* The following construct is necessary enable reporting of the       __NL__ \
   * type of the plugin that implemented a hook with a wrong signature. __NL__ \
   * This is necessary as it is not possible to include any non         __NL__ \
   * literal string constans in the error message of a static_assert.   __NL__ \
   */                                                                   __NL__ \
  constexpr bool dummy =                                                __NL__ \
    ___________Culprit_Plugin___________                                __NL__ \
       <PLUGIN, Check::value>::value;                                   __NL__ \
}
