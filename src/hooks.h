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

// This template is instanciated when something goes wrong. It causes a 
// compiler error as it does not define a constant 'value'.
//
template<typename Plugin__, bool result>
struct ___________________This_is_the_culprit_plugin_____________________
{};

// This specialization is instanciated when everything is ok.
//
template<typename Plugin__>
struct ___________________This_is_the_culprit_plugin_____________________
<Plugin__, true>
{
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
__NL__                                                                         \
__NL__  {                                                                      \
__NL__    /* Check it the two signatures equal. If not, the                    \
__NL__    * user has implemented a method with the same name but a             \
__NL__    * different (wrong) signature.                                       \
__NL__    */                                                                   \
__NL__    typedef HookSignaturesMatch<                                         \
__NL__                       decltype(&KaleidoscopePlugin::HOOK_METHOD),       \
__NL__                       decltype(&PLUGIN::HOOK_METHOD)                    \
__NL__                  > Check;                                               \
__NL__                                                                         \
__NL__    static_assert(Check::value,                                          \
__NL__     "*****************************************************************" \
__NL__     "******************** READ THIS CAREFULLY ! **********************" \
__NL__     "**************************************************************** " \
__NL__     "One of your plugins implemented a hook \"" #HOOK_METHOD            \
__NL__     "\" whose signature differs from the base class' hook signature. "  \
__NL__     "Please check the compiler messages above or below for an hint "    \
__NL__     "about which plugin is the culprit. "                               \
__NL__     "Then check all the hook methods implemented by the plugin "        \
__NL__     "and compare them to the respective hook methods of "               \
__NL__     "base class KaleidscopePlugin. Note: Two function signatures "      \
__NL__     "equal if all argument types and the method's "                     \
__NL__     "const qualifier equal. "                                           \
__NL__     "*****************************************************************" \
__NL__     "*****************************************************************" \
__NL__     "*****************************************************************" \
__NL__    );                                                                   \
__NL__                                                                         \
__NL__    /* The following construct is necessary enable reporting of the      \
__NL__     * type of the plugin that implemented a hook with a wrong signature.\
__NL__     * This is necessary as it is not possible to include any non        \
__NL__     * literal string constans in the error message of a static_assert.  \
__NL__     */                                                                  \
__NL__    constexpr bool dummy =                                               \
__NL__      ___________________This_is_the_culprit_plugin_____________________ \
__NL__         <PLUGIN, Check::value>::value;                                  \
__NL__  }
