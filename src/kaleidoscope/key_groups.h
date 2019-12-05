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

#include "kaleidoscope/key_defs.h"

static constexpr uint8_t encodeKeyGroupFlags(uint8_t key_group_flags) {

  // We use the unused bits in the key flags of layer change/toggle keycodes
  // to code the key groups that are affected by a layer operation.
  // Every bit thereby represents one keygroup.
  // Unfortunately, the unused bits that are available dont form
  // a contiguous part of the flags byte.
  //
  // The following bits of the key flags bitfield are already used:
  // SYNTHETIC | SWITCH_TO_KEYMAP
  //
  // Thus, the following bits (those set to one) B10111011 can be used
  // to code affected key groups. Every group is assigned one bit.
  // Because of this, only a maximum of six key groups can be defined.
  // If all flags are set to zero, this means the same as if all related
  // flags are set to one. This is necessary to provide backwards compatibility
  // for those layer toggle/switch commands that are not associated with
  // key groups. Those commands then mean "all groups affected".

  return (key_group_flags & B00000011)
         | ((key_group_flags & B00011100) << 1)
         | ((key_group_flags & B00100000) << 2);
}

static constexpr uint8_t decodeKeyGroupFlags(uint8_t coded_flags) {

  return (coded_flags & B00000011)
         | ((coded_flags & B00111000) >> 1)
         | ((coded_flags & B10000000) >> 2);
}

inline static constexpr Key KeyGroup(uint8_t key_group_flags, Key k) {
  return Key(k.getKeyCode(), k.getFlags() | encodeKeyGroupFlags(key_group_flags));
}

// Key group flags
static constexpr uint8_t KEY_GROUP_0 = B00000001;
static constexpr uint8_t KEY_GROUP_1 = B00000010;
static constexpr uint8_t KEY_GROUP_2 = B00000100;
static constexpr uint8_t KEY_GROUP_3 = B00001000;
static constexpr uint8_t KEY_GROUP_4 = B00010000;
static constexpr uint8_t KEY_GROUP_5 = B00100000;

// The default configuration of key grouping is that group 0 is assigned
// to the left hand, group 1 to the right. We provide constants to be used in
// user sketches.
static constexpr uint8_t KEY_GROUP_LEFT_HAND = KEY_GROUP_0;
static constexpr uint8_t KEY_GROUP_RIGHT_HAND = KEY_GROUP_1;

// This is also the default if no key group is assigned to a layer toggle
// keycode.
static constexpr uint8_t ALL_KEY_GROUPS = B00111111;

namespace kaleidoscope {

// The maximum number of possible key groups. This cannot be exceeded
// as there are not more than six unused bits in the key flags left to
// code the keygroup.
static constexpr uint8_t max_num_key_groups = 6;

static constexpr bool isKeyGroupFlagSet(uint8_t key_group_flags, uint8_t flag_id) {
  return key_group_flags & (B00000001 << flag_id);
}

} // end kaleidoscope

#define KEY_GROUP_IDS_STACKED(group_ids_per_key...) \
  const uint8_t key_groups[kaleidoscope_internal::device.numKeys()] PROGMEM = KEYMAP_STACKED(group_ids_per_key);

#define KEY_GROUP_IDS(group_ids_per_key...)				\
  const uint8_t key_groups[kaleidoscope_internal::device.numKeys()] PROGMEM = KEYMAP(group_ids_per_key);

#define GROUP_OF_KEY(key_addr) \
  pgm_read_byte(&(key_groups[key_addr.toInt()]))
