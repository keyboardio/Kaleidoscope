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

#include "kaleidoscope/macro_helpers.h"  // for __NL__

// The following code has been stolen from
//
// https://cpptalk.wordpress.com/2009/09/12/substitution-failure-is-not-an-error-2/
// Posted by: rmn on: 12/09/2009

// It defines a traits class that enables checking if a class
// defines a member (no matter if function or data member) with a given
// name. So what the class does is essentially checking if a equally
// named member in a helper base class (a mixin) is hidden by a
// reimplementation. The traits can by no means answer the question if
// the member that is implemented in the class being checked is
// a data or a function member. But for many applications that is of
// no great importance, which makes this technique very valuable.

// The code is highly complex that's why so many words are necessary to
// explain what it does. But the explanations are good enough to actually
// understand what's going on.

#define CAT3(A, B, C) A##B##C

#define DEFINE_HAS_MEMBER_TRAITS(PREFIX, MEMBER_NAME)                   __NL__ \
                                                                        __NL__ \
   /* This defines a templated class PREFIX##_HasMember_##MEMBER_NAME.*/__NL__ \
   /* The double hashes just glue together the value of macro        */ __NL__ \
   /* parameters like PREFIX or MEMBER_NAME and other                */ __NL__ \
   /* string tokens to form new identifiers (here the struct name).  */ __NL__ \
   /*/                                                               */ __NL__ \
   template<typename T>                                                 __NL__ \
   struct CAT3(PREFIX, _HasMember_, MEMBER_NAME) {                      __NL__ \
                                                                        __NL__ \
      /* This code defines an inner class, Fallback, with one        */ __NL__ \
      /* member – named MEMBER_NAME (remember that MEMBER_NAME is a  */ __NL__ \
      /* macro parameter, so it will be replaced with the name of the*/ __NL__ \
      /* actual member. Using proper terminology, this class is      */ __NL__ \
      /* essentially a Mixin.                                        */ __NL__ \
                                                                        __NL__ \
      struct Fallback {                                                 __NL__ \
         int MEMBER_NAME;                                               __NL__ \
      };                                                                __NL__ \
                                                                        __NL__ \
      /* Next, we introduce a new class: Derived. It inherits both   */ __NL__ \
      /* from the class T we’re templated on, & the previous Fallback*/ __NL__ \
      /* class. Note that the Derived class inherits the MEMBER_NAME */ __NL__ \
      /* member from class Fallback and possibly another MEMBER_NAME */ __NL__ \
      /* from T. Keep this (possible) ambiguity in mind.             */ __NL__ \
                                                                        __NL__ \
      struct Derived : T, Fallback { };                                 __NL__ \
                                                                        __NL__ \
      /* The ChT class is templated on a typename C, and an object   */ __NL__ \
      /* of that type. As a side note, only compile time constant    */ __NL__ \
      /* fixed types are possible here. We will use this class to    */ __NL__ \
      /* generate the ambiguity mentioned above.                     */ __NL__ \
                                                                        __NL__ \
      template<typename C, C>                                           __NL__ \
      struct ChT;                                                       __NL__ \
                                                                        __NL__ \
      /* This is where the fun starts. These two lines form a        */ __NL__ \
      /* declaration of a templated function f. That function        */ __NL__ \
      /* receives a pointer to the ChT class, instantiated with a    */ __NL__ \
      /* pointer-to-member type whose parameter is the               */ __NL__ \
      /* address-of-member MEMBER_NAME in class C. Note that         */ __NL__ \
      /* if we attempt to instantiate this with                      */ __NL__ \
      /* the Derived class, one of two things will happen: either we */ __NL__ \
      /* will have a substitution failure due to the aforementioned  */ __NL__ \
      /* ambiguity (if T also has a member by that name), or it will */ __NL__ \
      /* be successful (if there is no member called MEMBER_NAME, in T).*/ __NL__ \
      /* This function returns a reference to a char array of size 1.*/ __NL__ \
      /*                                                             */ __NL__ \
      /* How returning arrays by reference works becomes a little    */ __NL__ \
      /* clearer by means of a more simple example. The definition   */ __NL__ \
      /* of a function that returns a reference to a char-array of   */ __NL__ \
      /* size 1 e.g. reads                                           */ __NL__ \
      /*                                                             */ __NL__ \
      /* char (&f())[1];                                             */ __NL__ \
                                                                        __NL__ \
      template<typename C>                                              __NL__ \
      static char (&f(ChT<int Fallback::*, &C::MEMBER_NAME>*))[1];      __NL__ \
	                                                                __NL__ \
      /* This is what gets instantiated if the previous template     */ __NL__ \
      /* can’t be instantiated and SFINAE kicks in (since variadic   */ __NL__ \
      /* functions have the lowest possible priority when selecting  */ __NL__ \
      /* which overloaded function to call). This variadic function  */ __NL__ \
      /* returns a reference to a char array of size 2. Note that    */ __NL__ \
      /* instantiation of this function means that the previous      */ __NL__ \
      /* one failed, implying that class T has a member              */ __NL__ \
      /* named MEMBER_NAME.                                          */ __NL__ \
                                                                        __NL__ \
      template<typename C>                                              __NL__ \
      static char (&f(...))[2];                                         __NL__ \
                                                                        __NL__ \
      /* As you could guess, we will try to check which f function   */ __NL__ \
      /* can be instantiated. We will do this by checking the sizeof */ __NL__ \
      /* of the return value of that f. If the first signature did   */ __NL__ \
      /* not fit (could not be instantiated), then according to      */ __NL__ \
      /* SFINAE the second one will be the chosen one                */ __NL__ \
      /* (since it can’t fail) and we will take the sizeof of a      */ __NL__ \
      /* char array of size 2, which is 2. Therefore, the value      */ __NL__ \
      /* will be evaluated to true – meaning that class T really     */ __NL__ \
      /* has a member by the name MEMBER_NAME. Otherwise, the first  */ __NL__ \
      /* function can be successfully instantiated, and we will      */ __NL__ \
      /* measure the sizeof of a char array of size 1, which is 1.   */ __NL__ \
      /* In that case, the value is evaluated to false – which is    */ __NL__ \
      /* good for us sincethat means there was no ambiguity when     */ __NL__ \
      /* using Derived::MEMBER_NAME, and that means there was no     */ __NL__ \
      /* other MEMBER_NAME but the one in Fallback.                  */ __NL__ \
                                                                        __NL__ \
      static bool const value = sizeof(f<Derived>(0)) == 2;             __NL__ \
};
