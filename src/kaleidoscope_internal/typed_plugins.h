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

#include <stdint.h>

#include "kaleidoscope/macro_map.h"

// The following is template code that generates an array of pointers of
// a specific type of plugins (e.g. LEDMode's) at compiletime and can be
// stored in PROGMEM.

namespace kaleidoscope {
namespace internal {
namespace typed_plugins {

// The TypedPluginArray class stores pointers to plugins of a specific
// type in a recursive fashion.
//
template<typename TargetType__,
         bool IsAppropriateType__,
         bool...MoreTypeInfo__>
class TypedPluginArray {

 public:

  typedef TypedPluginArray<TargetType__, MoreTypeInfo__...> NestedArray;

  template<typename...MorePlugins__>
  constexpr TypedPluginArray(TargetType__ *entry, MorePlugins__...more_plugins)
    : entry_(entry),
      nested_array_(more_plugins...) {}

  static constexpr uint8_t n_entries
    = NestedArray::n_entries + 1;

  typedef TargetType__ ContentType;

 private:
  TargetType__ *entry_;
  NestedArray nested_array_;
} __attribute__((packed)); // Make sure that there are no padding
// bytes added by the compiler.
// This is important to let the class
// have the same layout as a POD array.


template<typename TargetType__,
         bool...MoreTypeInfo__>
class TypedPluginArray<TargetType__,
        false /* Not of appropriate type */,
        MoreTypeInfo__...> {

 public:

  typedef TypedPluginArray<TargetType__, MoreTypeInfo__...> NestedArray;

  template<typename...MorePlugins__>
  constexpr TypedPluginArray(void */* non-matching plugin */,
                             MorePlugins__...more_plugins)
    : nested_array_(more_plugins...) {}

  static constexpr uint8_t n_entries
    = NestedArray::n_entries;

  typedef TargetType__ ContentType;

 private:

  NestedArray nested_array_;
} __attribute__((packed));

template<typename TargetType__>
struct TypedPluginArray<TargetType__, true /* is of appropriate type */> {

 public:

  constexpr TypedPluginArray(TargetType__ *entry)
    : entry_(entry)
  {}

  static constexpr uint8_t n_entries = 1;

  typedef TargetType__ ContentType;

 private:
  TargetType__ *entry_;
} __attribute__((packed));

template<typename TargetType__>
struct TypedPluginArray<TargetType__, false /* not of appropriate type */> {

 public:

  constexpr TypedPluginArray(void */* non-matching plugin */) {}

  static constexpr uint8_t n_entries = 0;

  typedef TargetType__ ContentType;
} __attribute__((packed));

// This function reads an entry of an auto generated TypedPluginArray
// by treating TypedPluginArray as an ordinary contiguous 1D array
//
template<typename ArrayType__>
auto getEntryFromPROGMEM(const ArrayType__ &array, uint8_t n)
// Query the content type of the array argument (this is also a safety
// check as the compile would fail if a nested ContentType would not
// be defined
-> typename ArrayType__::ContentType * {
  // C++ requires "typename" to be explicily stated for retreiving
  // nested typedefs from template parameters.
  //
  typedef typename ArrayType__::ContentType TargetType;

  return reinterpret_cast<TargetType*>(
           pgm_read_word(&reinterpret_cast<TargetType *const*>(&array)[n])
         );
}

} // end namespace typed_plugins
} // end namespace internal
} // end namespace kaleidoscope

#define _RETREIVE_ADDRESS(PLUGIN) &::PLUGIN
#define _IS_APPROPRIATE_TYPE(PLUGIN) isAppropriateType(&::PLUGIN)

// This macro is meant for internal use only
//
#define _GENERATE_TYPED_PLUGIN_ARRAY(PLUGIN_TYPE, ...)                         \
   /* Wrap as many internal symbols as possible*/                       __NL__ \
   namespace kaleidoscope {                                             __NL__ \
   namespace internal {                                                 __NL__ \
   namespace typed_plugins {                                            __NL__ \
   namespace PLUGIN_TYPE {                                              __NL__ \
                                                                        __NL__ \
      /* The following two auxiliar functions help to check             __NL__ \
       * if a plugin is of appropriate type at compile time */          __NL__ \
      constexpr bool isAppropriateType(                                 __NL__ \
                 kaleidoscope::plugin::PLUGIN_TYPE *) { return true; }  __NL__ \
      constexpr bool isAppropriateType(void *) { return false; }        __NL__ \
                                                                        __NL__ \
      /* Determine the actual type of the TypedPluginArray template     __NL__ \
       * instance that is generated by using the decltype operator      __NL__ \
       * at compile time */                                             __NL__ \
      typedef TypedPluginArray<kaleidoscope::plugin::PLUGIN_TYPE,       __NL__ \
                            /* Turn the list of global plugin objects   __NL__ \
                             * into a list of their addresses */        __NL__ \
                            MAP_LIST(_IS_APPROPRIATE_TYPE, __VA_ARGS__) __NL__ \
              > ArrayType;                                              __NL__ \
                                                                        __NL__ \
      /* Generate the actual instance of template class                 __NL__ \
       * TypedPluginArray and initialize it with a list of pointers     __NL__ \
       * to all specified plugins. The generateArray function           __NL__ \
       * takes care that only those plugins are filtered out whose      __NL__ \
       * type matches.                                                  __NL__ \
       */                                                               __NL__ \
      const PROGMEM ArrayType array(                                    __NL__ \
                            MAP_LIST(_RETREIVE_ADDRESS, __VA_ARGS__)    __NL__ \
               );                                                       __NL__ \
                                                                        __NL__ \
      /* Store the number of typed plugins in PROGMEM. This             __NL__ \
       * variable lives in internal namespace as there is a             __NL__ \
       * inlined convenience retreival function provided                __NL__ \
       * that takes care of the PROGMEM access.                         __NL__ \
       */                                                               __NL__ \
      const PROGMEM uint8_t num_entries                                 __NL__ \
              = ArrayType::n_entries;                                   __NL__ \
                                                                        __NL__ \
   } /* end namespace PLUGIN_TYPE */                                    __NL__ \
   } /* end namespace typed_plugins */                                  __NL__ \
   } /* end namespace internal */                                       __NL__ \
   } /* end namespace kaleidoscope */                                   __NL__ \
                                                                        __NL__ \
   /* Have a retreival function that enables convenient access to       __NL__ \
    * our list of typed plugins.                                        __NL__ \
    */                                                                  __NL__ \
   kaleidoscope::plugin::PLUGIN_TYPE *get##PLUGIN_TYPE(uint8_t n) {     __NL__ \
      return kaleidoscope::internal                                     __NL__ \
         ::typed_plugins::getEntryFromPROGMEM(                          __NL__ \
            kaleidoscope::internal::typed_plugins::PLUGIN_TYPE::array,  __NL__ \
            n);                                                         __NL__ \
   }

// This macro is meant to be used in plugins that require access to
// a typed array of specific type
#define PROVIDE_ACCESS_TO_PLUGINS_OF_TYPE(PLUGIN_TYPE)                         \
   /* Forward declare to avoid a header inclusion.*/                    __NL__ \
   namespace kaleidoscope {                                             __NL__ \
   namespace plugin {                                                   __NL__ \
      class PLUGIN_TYPE;                                                __NL__ \
   } /* end namespace plugin */                                         __NL__ \
   } /* end namespace kaleidoscope */                                   __NL__ \
                                                                        __NL__ \
   /* Wrap as many internal symbols as possible*/                       __NL__ \
   namespace kaleidoscope {                                             __NL__ \
   namespace internal {                                                 __NL__ \
   namespace typed_plugins {                                            __NL__ \
   namespace PLUGIN_TYPE {                                              __NL__ \
      extern const uint8_t num_entries;                                 __NL__ \
   } /* end namespace PLUGIN_TYPE */                                    __NL__ \
   } /* end namespace typed_plugins */                                  __NL__ \
   } /* end namespace internal */                                       __NL__ \
   } /* end namespace kaleidoscope */                                   __NL__ \
                                                                        __NL__ \
   extern kaleidoscope::plugin::PLUGIN_TYPE *                           __NL__ \
               get##PLUGIN_TYPE(uint8_t n);                             __NL__ \
                                                                        __NL__ \
   inline uint8_t num##PLUGIN_TYPE##s() {                               __NL__ \
      return pgm_read_byte(&kaleidoscope::internal                      __NL__ \
                     ::typed_plugins::PLUGIN_TYPE::num_entries);        __NL__ \
   }
