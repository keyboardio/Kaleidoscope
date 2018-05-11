#pragma once

#include "macro_helpers.h"
#include "plugin.h"


// *************************************************************************
// *************************************************************************
// NOTHING IN THIS HEADER WILL RESULT IN ANY CODE COMPILED INTO KALEIDOSCOPE
//
// YOU DO NOT NEED TO LOOK INSIDE THIS FILE IF YOU'RE TRYING TO UNDERSTAND
// HOW KALEIDOSCOPE WORKS.
// *************************************************************************
// *************************************************************************

// This header file implements compile-time method signature verification for
// plugins' event handlers as the _VALIDATE_EVENT_HANDLER_SIGNATURE macro.

// If the method signatures don't match the API definitions, compilation
// should fail with a helpful error message.

// *************************************************************************
// *************************************************************************

// We use non-virtual methods for event handlers. This reduces RAM (and to a
// smaller extent, PROGMEM) consumption, runtime call overhead, and allows the
// compiler to a better job removing dead code.
//
// The downside is that event handler methods in derived plugins are hidden,
// rather than overridden. Because only virtual methods can be overridden, we
// can't apply C++'s `override` keyword to hook methods.
//
// To make it easier to debug issues with plugins' event handler method
// signatures, the _VALIDATE_EVENT_HANDLER_SIGNATURE macro compares a plugin's
// event handler signatures with those of the kaleidoscope::Plugin baseclass. If
// any differences are detected, it outputs a compile-time error message.
//
// Ideally, we'd be able to use this (much simpler) code.
//
//    decltype(&::kaleidoscope::Plugin::fooEventHandler)
//              == decltype(&childPlugin::fooEventHandler)
//
// Unfortunately, this is not possible with C++-11, as it does not allow
// comparing function-types for equality. As a workaround, we can use the trait
// class EventHandlerSignaturesMatch to perform the comparison.
//
// It defines the bool constant 'value' as true if both signatures match.
// It checks the return value, argument types and const specification.
// (As of this writing, we don't have any const hook method.)

template<typename T1, typename T2>
struct EventHandlerSignaturesMatch {
  static constexpr bool value = false;
};

// R: The return value,
// T1: Type of the first class (plugin),
// T2: Type of the second class (plugin),
// EventHandlerArgs: Variadic types of plugin event handler arguments.

template<typename R, typename T1, typename T2, typename... EventHandlerArgs>
struct EventHandlerSignaturesMatch<R(T1::*)(EventHandlerArgs...), R(T2::*)(EventHandlerArgs...)> {
  static constexpr bool value = true;
};

// Equivalent to allow for const-eventhandlers, e.g. bool getFooEventHandler() const
template<typename R, typename T1, typename T2, typename... EventHandlerArgs> struct
EventHandlerSignaturesMatch<R(T1::*)(EventHandlerArgs...) const, R(T2::*)(EventHandlerArgs...) const> {
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

#define _VALIDATE_EVENT_HANDLER_SIGNATURE(EVENTHANDLER, PLUGIN)                  \
{                                                                         __NL__ \
  /* Check if the signatures match. If not, the plugin has implemented */ __NL__ \
  /* a method with a hook's name, but a different signature.           */ __NL__ \
  typedef EventHandlerSignaturesMatch<                                    __NL__ \
                     decltype(&::kaleidoscope::Plugin::EVENTHANDLER),     __NL__ \
                     decltype(&PLUGIN::EVENTHANDLER)                      __NL__ \
                > Check;                                                  __NL__ \
                                                                          __NL__ \
  static_assert(Check::value,                                             __NL__ \
    VERBOSE_STATIC_ASSERT_HEADER                                          __NL__ \
   "\nOne of your plugins implemented the \"" #EVENTHANDLER "\""          __NL__ \
   "\nevent handler, but its signature didn't match the base class."      __NL__ \
   "\n"                                                                   __NL__ \
   "\nThe plugin with this issue will be marked in the compiler"          __NL__ \
   "\noutput with the string:"                                            __NL__ \
   "\n"                                                                   __NL__ \
   "\n      ___________Culprit_Plugin___________."                        __NL__ \
   "\n"                                                                   __NL__ \
   "\nYou should compare the event handlers implemented in this plugin"   __NL__ \
   "\nto those in \"kaleidoscope::Plugin\"."                              __NL__ \
   "\n"                                                                   __NL__ \
   "\nAll of the event handler's argument types, return values and"       __NL__ \
   "\nconst qualifiers need to match."                                    __NL__ \
   "\n"                                                                   __NL__ \
    VERBOSE_STATIC_ASSERT_FOOTER                                          __NL__ \
  );                                                                      __NL__ \
                                                                          __NL__ \
  /* The following construct is necessary enable reporting of the      */ __NL__ \
  /* type of a plugin that implements an event handler with an         */ __NL__ \
  /* incorrect signature, because it's not possible to include any     */ __NL__ \
  /* non-literal string constant in a static_assert error message.     */ __NL__ \
  constexpr bool dummy = ___________Culprit_Plugin___________             __NL__ \
       <PLUGIN, Check::value>::value;                                     __NL__ \
}
