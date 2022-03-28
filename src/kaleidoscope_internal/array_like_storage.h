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

#include <stdint.h>  // for uint8_t

namespace kaleidoscope {
namespace internal {

// The ArrayLikeStorage class stores data of a specific
// type in a recursive fashion. It is used to generate array like
// data structures at compile time that are initialized from a
// list of entities only a subset of which is supposed to be
// stored in the container.
//
template<typename StoredType__,
         bool IsAppropriateType__,
         bool... MoreTypeInfo__>
class ArrayLikeStorage {

 public:
  typedef ArrayLikeStorage<StoredType__, MoreTypeInfo__...> NestedArray;

  template<typename... MoreEntities__>
  constexpr ArrayLikeStorage(StoredType__ entry, MoreEntities__... more_entities)
    : entry_(entry),
      nested_array_(more_entities...) {}

  static constexpr uint8_t n_entries = NestedArray::n_entries + 1;

  typedef StoredType__ ContentType;

 private:
  StoredType__ entry_;
  NestedArray nested_array_;
} __attribute__((packed));  // Make sure that there are no padding
// bytes added by the compiler.
// This is important to let the class
// have the same layout as a POD array.


template<typename StoredType__,
         bool... MoreTypeInfo__>
class ArrayLikeStorage<StoredType__,
                       false /* Not of appropriate type */,
                       MoreTypeInfo__...> {

 public:
  typedef ArrayLikeStorage<StoredType__, MoreTypeInfo__...> NestedArray;

  template<typename AnyType__, typename... MoreEntities__>
  constexpr ArrayLikeStorage(AnyType__ /* non-matching entity */,
                             MoreEntities__... more_entities)
    : nested_array_(more_entities...) {}

  static constexpr uint8_t n_entries = NestedArray::n_entries;

  typedef StoredType__ ContentType;

 private:
  NestedArray nested_array_;
} __attribute__((packed));

template<typename StoredType__>
struct ArrayLikeStorage<StoredType__, true /* is of appropriate type */> {

 public:
  explicit constexpr ArrayLikeStorage(StoredType__ entry)
    : entry_(entry) {}

  static constexpr uint8_t n_entries = 1;

  typedef StoredType__ ContentType;

 private:
  StoredType__ entry_;
} __attribute__((packed));

template<typename StoredType__>
struct ArrayLikeStorage<StoredType__, false /* not of appropriate type */> {

 public:
  template<typename AnyType__>
  explicit constexpr ArrayLikeStorage(AnyType__ /* non-matching entity */) {}

  static constexpr uint8_t n_entries = 0;

  typedef StoredType__ ContentType;
} __attribute__((packed));

}  // namespace internal
}  // namespace kaleidoscope
