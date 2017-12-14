#pragma once

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
//    decltype(&KaleidoscopePlugin::fooHookMethod)
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
//
template<typename T1, typename T2>
struct HookSignaturesMatch {
  static constexpr bool value = false;
};

// R: The return value, T1: Type of the first class (plugin),
// T2: Type of the second class (plugin), HookArgs: Variadic types of
// plugin hook arguments.
//
template<typename R, typename T1, typename T2, typename... HookArgs>
struct HookSignaturesMatch<R(T1::*)(HookArgs...), R(T2::*)(HookArgs...)> {
  static constexpr bool value = true;
};

// Equivalent to allow for const-hooks, e.g. bool getFooHook() const
//
template<typename R, typename T1, typename T2, typename... HookArgs>
struct HookSignaturesMatch<R(T1::*)(HookArgs...) const, R(T2::*)(HookArgs...) const> {
  static constexpr bool value = true;
};

// The above specialized HookSignaturesMatch plugin class is instanciated
// based on two types of pointers to the plugin base class hook method and
// the derived method. If both pointer types and thus the signatures equal,
// the first or second specialization is instanciated,
// leading to its compile time constant 'value' being defined as true.
// Else the unspecialized version of the template class is instantiated,
// causing 'value' to be defined as false.

#define MATCH_HOOK_TYPE(HOOK_METHOD, PLUGIN)                                   \
__NN__                                                                         \
__NN__  {                                                                      \
__NN__    /* Check it the two signatures equal. If not, the                    \
__NN__    * user has implemented a method with the same name but a             \
__NN__    * different (wrong) signature.                                       \
__NN__    */                                                                   \
__NN__    static_assert(HookSignaturesMatch<                                   \
__NN__                       decltype(&KaleidoscopePlugin::HOOK_METHOD),       \
__NN__                       decltype(&PLUGIN::HOOK_METHOD)                    \
__NN__                  >::value,                                              \
__NN__     "*****************************************************************" \
__NN__     "******************** READ THIS CAREFULLY ! **********************" \
__NN__     "**************************************************************** " \
__NN__     "The signature of hook \"" #HOOK_METHOD                             \
__NN__     "\" differs from the base class' hook signature. "                  \
__NN__     "Please check all the hook methods implemented by your plugins "    \
__NN__     "and compare them to the respective hook methods of "               \
__NN__     "base class KaleidscopePlugin. Note: Two function signatures "      \
__NN__     "equal if all argument types and the method's "                     \
__NN__     "const qualifier equal. "                                           \
__NN__     "*****************************************************************" \
__NN__     "*****************************************************************" \
__NN__     "*****************************************************************" \
__NN__    );                                                                   \
__NN__  }