/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope/plugin.h"
#include "kaleidoscope_internal/type_traits/has_member.h"

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
// comparing function-types for equality. This code relies on several
// template traits classes to check if a function's call signature matches.

// This template is instantiated when something goes wrong.
// Because it does not define a constant 'value', it triggers a compiler error.

template<typename Plugin__, bool result> struct
  ___________Culprit_Plugin___________ { };

// This specialization is instantiated when everything is ok.
template<typename Plugin__> struct
  ___________Culprit_Plugin___________ <Plugin__, true> {
  static constexpr bool value = true;
};

#define _NOOP(...)   __NL__ \

#define _DEFINE_IMPLEMENTATION_CHECK_CLASSES(HOOK_NAME, ...)                   \
                                                                        __NL__ \
   /* Defines a traits class Plugin_HasMember_HOOK_NAME                 __NL__ \
    * where HOOK_NAME is replaced with the actual hook name             __NL__ \
    * that is passed in as a parameter to this macro.                   __NL__ \
    */                                                                  __NL__ \
   DEFINE_HAS_MEMBER_TRAITS(Plugin, HOOK_NAME)                          __NL__ \
                                                                        __NL__ \
   /* Specializations of this helper class check if a plugin            __NL__ \
    * implements a handler with the specific signature that is          __NL__ \
    * identified by a hook version.                                     __NL__ \
    */                                                                  __NL__ \
   template<typename Plugin__, int hook_version_id_>                    __NL__ \
   struct HookVersionImplemented_##HOOK_NAME {};

#define _DEFINE_IMPLEMENTATION_CHECK_CLASS_SPECIALIZATION(                     \
    HOOK_NAME, HOOK_VERSION, DEPRECATION_TAG,                                  \
    SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST)                      \
                                                                        __NL__ \
   /* This specialization checks if a plugin of type Plugin__           __NL__ \
    * implements a handler with given signature SIGNATURE.              __NL__ \
    */                                                                  __NL__ \
   template<typename Plugin__>                                          __NL__ \
   struct HookVersionImplemented_##HOOK_NAME<Plugin__, HOOK_VERSION>    __NL__ \
   {                                                                    __NL__ \
      /* Define a pointer to member function with the correct           __NL__ \
       * argument signature. The newly defined type is named            __NL__ \
       * HookType__.                                                    __NL__ \
       */                                                               __NL__ \
      typedef kaleidoscope::EventHandlerResult                          __NL__ \
                (Plugin__::*HookType__)SIGNATURE;                       __NL__ \
                                                                        __NL__ \
      /* See the definition of HookIimplemented_##HOOK_NAME above       __NL__ \
       * for an explanation of the SFINAE concept that is applied here. __NL__ \
       * The difference to the afforementioned class is that            __NL__ \
       * here we check if a specific version of a handler was           __NL__ \
       * implemented. This is done by forcing the compiler              __NL__ \
       * through a static cast to select the respective method          __NL__ \
       * if possible. If the method signature cannot be found,          __NL__ \
       * the substitution fails and the first version of method "test"  __NL__ \
       * will not be defined.                                           __NL__ \
       */                                                               __NL__ \
      template <typename PluginAux__>                                   __NL__ \
      static constexpr                                                  __NL__ \
      /* The following decltype-clause defines the function return type __NL__ \
       */                                                               __NL__ \
      decltype(                                                         __NL__ \
         /* If &PluginAux__::HOOK_NAME exists and is of type            __NL__ \
          * HookType__, the list below evaluates to bool{} whose        __NL__ \
          * type can be determined. Otherwise the comma expression      __NL__ \
          * cannot be evaluated and the content                         __NL__ \
          * of decltype is undefined and this function overload         __NL__ \
          * is ignored by the compiler                                  __NL__ \
          * (SFINAE = substitution failure is not an error)             __NL__ \
          * and the test(...) overload is used instead.                 __NL__ \
          */                                                            __NL__ \
         static_cast<HookType__>(&PluginAux__::HOOK_NAME), bool{}       __NL__ \
      )                                                                 __NL__ \
      test(int /* unused */)                                            __NL__ \
      {                                                                 __NL__ \
         return true;                                                   __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      template <typename PluginAux__>                                   __NL__ \
      static constexpr bool test(...)                                   __NL__ \
      {                                                                 __NL__ \
         return false;                                                  __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      static constexpr bool value = test<Plugin__>(int{});              __NL__ \
   };

#define _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK_START(HOOK_NAME, ...)           \
                                                                        __NL__ \
   /* This struct enables checking if a handler of a specific type      __NL__ \
    * has been implemented. If so, there must be exactly one overload   __NL__ \
    * with correct signature.                                           __NL__ \
    */                                                                  __NL__ \
   template <class Plugin__>                                            __NL__ \
   struct NumberOfImplementationsOf_##HOOK_NAME                         __NL__ \
   {                                                                    __NL__ \
      static constexpr int value =                                      __NL__ \
         0                                                              __NL__ \
         /* What follows is a list of handler implementation            __NL__ \
          * checks for different versions of a handler                  __NL__ \
          * generated by several calls to                               __NL__ \
          * _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK_OP.                  __NL__ \
          */

// This is invoked for every version of a hook.
//
#define _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK_OP(HOOK_NAME, HOOK_VERSION)     \
   +  ((HookVersionImplemented_##HOOK_NAME<Plugin__,                    __NL__ \
                                             HOOK_VERSION>::value       __NL__ \
      ) ? 1 : 0)

#define _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK_END(HOOK_NAME, ...)             \
      ;                                                                 __NL__ \
   };

#define _VALIDATE_EVENT_HANDLER_SIGNATURE(HOOK_NAME, PLUGIN_TYPE)              \
{                                                                       __NL__ \
   static constexpr bool derived_implements_handler                     __NL__ \
         = Plugin_HasMember_##HOOK_NAME<PLUGIN_TYPE>::value;            __NL__ \
                                                                        __NL__ \
   static constexpr int n_implementations                               __NL__ \
         = NumberOfImplementationsOf_##HOOK_NAME<PLUGIN_TYPE>::value;   __NL__ \
                                                                        __NL__ \
   /* A handler is acceptable if it is either not implemented           __NL__ \
      * by the derived class or if there is only one implementation     __NL__ \
      * with correct signature.                                         __NL__ \
      * Please note that any other methods with different names         __NL__ \
      * but other, unrelated signatures are ignored as long             __NL__ \
      * as there is one correct implementation.                         __NL__ \
      * Only if there are several versions supported at a time          __NL__ \
      * and more than one of them has been implemented, we              __NL__ \
      * treat this as an ambiguity and raise an error.                  __NL__ \
      */                                                                __NL__ \
   static constexpr bool handler_has_wrong_signature =                  __NL__ \
         derived_implements_handler && (n_implementations == 0);        __NL__ \
                                                                        __NL__ \
  static_assert(!handler_has_wrong_signature,                           __NL__ \
    VERBOSE_STATIC_ASSERT_HEADER                                        __NL__ \
   "\nOne of your plugins implemented the \"" #HOOK_NAME "\""           __NL__ \
   "\nevent handler, but its signature didn't match."                   __NL__ \
   "\n"                                                                 __NL__ \
   "\nThe plugin with this issue will be marked in the compiler"        __NL__ \
   "\noutput with the string:"                                          __NL__ \
   "\n"                                                                 __NL__ \
   "\n      ___________Culprit_Plugin___________."                      __NL__ \
   "\n"                                                                 __NL__ \
   "\nYou should compare the event handlers implemented in this plugin" __NL__ \
   "\nto those listed in the file event_handlers.h."                    __NL__ \
   "\n"                                                                 __NL__ \
   "\nPlease note that some of the handlers might have multiple"        __NL__ \
   "\nversions. Is it possible that you implemented several versions"   __NL__ \
   "\nof the same handler?"                                             __NL__ \
   "\n"                                                                 __NL__ \
   "\nAll of the event handler's argument types, return values and"     __NL__ \
   "\nconst qualifiers need to match."                                  __NL__ \
   "\n"                                                                 __NL__ \
    VERBOSE_STATIC_ASSERT_FOOTER                                        __NL__ \
  );                                                                    __NL__ \
                                                                        __NL__ \
  /* The following construct is necessary enable reporting of the       __NL__ \
   * type of a plugin that implements an event handler with an          __NL__ \
   * incorrect signature, because it's not possible to include any      __NL__ \
   * non-literal string constant in a static_assert error message.      __NL__ \
   */                                                                   __NL__ \
  __attribute__((unused)) constexpr bool dummy1                         __NL__ \
                       = ___________Culprit_Plugin___________           __NL__ \
       <PLUGIN_TYPE, !handler_has_wrong_signature>::value;              __NL__ \
                                                                        __NL__ \
   static constexpr bool handler_ambiguously_implemented =              __NL__ \
         derived_implements_handler && (n_implementations > 1);         __NL__ \
                                                                        __NL__ \
   static_assert(!handler_ambiguously_implemented,                      __NL__ \
    VERBOSE_STATIC_ASSERT_HEADER                                        __NL__ \
   "\nOne of your plugins implemented multiple versions of the"         __NL__ \
   "\n\"" #HOOK_NAME "\" event handler."                                __NL__ \
   "\n"                                                                 __NL__ \
   "\nThe plugin with this issue will be marked in the compiler"        __NL__ \
   "\noutput with the string:"                                          __NL__ \
   "\n"                                                                 __NL__ \
   "\n      ___________Culprit_Plugin___________."                      __NL__ \
   "\n"                                                                 __NL__ \
   "\nYou should compare the event handlers implemented in this plugin" __NL__ \
   "\nto those listed in the file event_handlers.h."                    __NL__ \
   "\n"                                                                 __NL__ \
   "\nPlease make sure that one version is implemented in your plugin." __NL__ \
   "\n"                                                                 __NL__ \
    VERBOSE_STATIC_ASSERT_FOOTER                                        __NL__ \
  );                                                                    __NL__ \
                                                                        __NL__ \
  /* The following construct is necessary enable reporting of the       __NL__ \
   * type of a plugin that implements an event handler with an          __NL__ \
   * incorrect signature, because it's not possible to include any      __NL__ \
   * non-literal string constant in a static_assert error message.      __NL__ \
   */                                                                   __NL__ \
  __attribute__((unused)) constexpr bool dummy2                         __NL__ \
                       = ___________Culprit_Plugin___________           __NL__ \
       <PLUGIN_TYPE, !handler_has_wrong_signature>::value;              __NL__ \
}

#define _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK                                 \
  _PROCESS_EVENT_HANDLER_VERSIONS(                                      __NL__ \
     _DEFINE_IMPLEMENTATION_CHECK_CLASSES,                              __NL__ \
     _NOOP,                                                             __NL__ \
     _NOOP                                                              __NL__ \
  )                                                                     __NL__ \
                                                                        __NL__ \
  _FOR_EACH_EVENT_HANDLER(                                              __NL__ \
     _DEFINE_IMPLEMENTATION_CHECK_CLASS_SPECIALIZATION)                 __NL__ \
                                                                        __NL__ \
  _PROCESS_EVENT_HANDLER_VERSIONS(                                      __NL__ \
     _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK_START,                      __NL__ \
     _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK_OP,                         __NL__ \
     _PREPARE_EVENT_HANDLER_SIGNATURE_CHECK_END                         __NL__ \
  )
