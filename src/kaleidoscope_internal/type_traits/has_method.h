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

// clang-format off

#pragma once

#include "kaleidoscope/macro_helpers.h"  // for __NL__, UNWRAP, ADD_TEMPLATE_BRACES, GLUE3, TEMP...

#define DEFINE_HAS_METHOD_TRAITS(PREFIX,                                       \
                                 TMPL_PARAM_TYPE_LIST, TMPL_PARAM_LIST,        \
                                 METHOD_NAME,                                  \
                                 RETURN_TYPE, ARGUMENTS)                       \
                                                                        __NL__ \
   /* This traits checks if a class of type Class__                   */ __NL__ \
   /* implements a method with given signature.                       */ __NL__ \
                                                                        __NL__ \
   template<typename Class__ UNWRAP TMPL_PARAM_TYPE_LIST>               __NL__ \
   struct GLUE3(PREFIX, _HasMethod_, METHOD_NAME)                       __NL__ \
   {                                                                    __NL__ \
      /* Define a pointer to member function with the correct        */ __NL__ \
      /* argument signature. The newly defined type is named         */ __NL__ \
      /* MethodType__.                                               */ __NL__ \
      /* */                                                             __NL__ \
      typedef RETURN_TYPE (Class__::*MethodType__)ARGUMENTS;            __NL__ \
                                                                        __NL__ \
      /* This is an application of the SFINAE concept.                */ __NL__ \
      /* We check if Class__ defines a method with given name and     */ __NL__ \
      /* signature. This is done by forcing the compiler              */ __NL__ \
      /* through a static cast to select the respective method        */ __NL__ \
      /* if possible. If the method signature cannot be found,        */ __NL__ \
      /* the substitution fails and the first version of method "test"*/ __NL__ \
      /* will not be defined.                                         */ __NL__ \
                                                                        __NL__ \
      template <typename ClassAux__>                                    __NL__ \
      static constexpr                                                  __NL__ \
      /* The following decltype-clause defines the function return type */ __NL__ \
                                                                        __NL__ \
      decltype(                                                         __NL__ \
         /* If &ClassAux__::METHOD_NAME exists and is of type        */ __NL__ \
         /* MethodType__, the list below evaluates to bool{} whose   */ __NL__ \
         /* type can be determined. Otherwise the comma expression   */ __NL__ \
         /* cannot be evaluated and the content                      */ __NL__ \
         /* of decltype is undefined and this function overload      */ __NL__ \
         /* is ignored by the compiler                               */ __NL__ \
         /* (SFINAE = substitution failure is not an error)          */ __NL__ \
         /* and the test(...) overload is used instead.              */ __NL__ \
                                                                        __NL__ \
         static_cast<MethodType__>(                                     __NL__ \
            &ClassAux__::TEMPLATE_KEYWORD(UNWRAP TMPL_PARAM_LIST)       __NL__ \
               METHOD_NAME ADD_TEMPLATE_BRACES(UNWRAP TMPL_PARAM_LIST)  __NL__ \
         ), bool{}                                                      __NL__ \
      )                                                                 __NL__ \
      test(int /* unused */) /* NOLINT(readability/casting) */          __NL__ \
      {                                                                 __NL__ \
         return true;                                                   __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      template <typename ClassAux__>                                    __NL__ \
      static constexpr bool test(...)                                   __NL__ \
      {                                                                 __NL__ \
         return false;                                                  __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      static constexpr bool value = test<Class__>(int{});               __NL__ \
   };
