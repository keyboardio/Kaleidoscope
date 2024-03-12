/* -*- mode: c++ -*-
 * Kaleidoscope-Ranges -- Common ranges, used by a number of Kaleidoscope plugins.
 * Copyright (C) 2016, 2017, 2019  Keyboard.io, Inc
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

#include <stdint.h>  // for uint16_t, uint8_t

// Included for definition of legacy Macros plugin key range:
#include "kaleidoscope/key_defs.h"  // for SYNTHETIC

namespace kaleidoscope {
namespace ranges {

// NOTE: We need to keep the ranges stable, and backwards compatible!
//
// When adding, removing, or changing ranges, make sure that existing ranges are
// never accidentally moved. If migrating keycodes that weren't previously using
// the range system, make sure you don't change the `Key` values. If an existing
// `Key` value is changed, it won't be a problem for the Kaleidoscope sketch
// itself, but it will cause problems for keymap entries stored in EEPROM
// (i.e. Chrysalis keymap layers), which will not get updated by flashing the
// firmware.
//
// When adding new `Key` values for plugins, to keep them backwards-compatible,
// they must be added at the end of the range below (just before `SAFE_START`),
// even if those values are logically related to existing ones. This is
// important for compatibility with existing Chrysalis keymaps, despite the fact
// that it makes the code more obtuse here.

constexpr uint8_t MAX_MACROS         = 255;
constexpr uint8_t MAX_OSM_KEYS       = 7;
constexpr uint8_t MAX_OSL_KEYS       = 7;
constexpr uint16_t MAX_DUM_KEYS      = 2048;
constexpr uint16_t MAX_DUL_KEYS      = 2048;
constexpr uint8_t MAX_TD_KEYS        = 15;
constexpr uint8_t MAX_LEAD_KEYS      = 7;
constexpr uint8_t MAX_TT_KEYS        = 255;
constexpr uint8_t MAX_STENO_KEYS     = 42;
constexpr uint8_t MAX_DYNAMIC_MACROS = 31;
constexpr uint8_t MAX_CS_KEYS        = 64;
constexpr uint8_t MAX_QK_KEYS        = 64;

enum : uint16_t {
  // Macro ranges pre-date Kaleidoscope-Ranges, so they're coming before
  // ranges::FIRST, because we want to keep the keycodes backwards compatible.
  // This is undesirable, because it prevents us from making a clear distinction
  // between plugin key values and core key values. The magic number
  // `0b00100000` is the old `IS_MACRO` key flags bit.
  MACRO_FIRST = (SYNTHETIC | 0b00100000) << 8,
  MACRO_LAST  = MACRO_FIRST + MAX_MACROS,

  FIRST              = 0xc000,
  KALEIDOSCOPE_FIRST = FIRST,
  OS_FIRST,
  OSM_FIRST = OS_FIRST,
  OSM_LAST  = OSM_FIRST + MAX_OSM_KEYS,
  OSL_FIRST,
  OSL_LAST = OSL_FIRST + MAX_OSL_KEYS,
  OS_LAST  = OSL_LAST,
  DU_FIRST,
  DUM_FIRST = DU_FIRST,
  DUM_LAST  = DUM_FIRST + MAX_DUM_KEYS,
  DUL_FIRST,
  DUL_LAST = DUL_FIRST + MAX_DUL_KEYS,
  DU_LAST  = DUL_LAST,
  TD_FIRST,
  TD_LAST = TD_FIRST + MAX_TD_KEYS,
  LEAD_FIRST,
  LEAD_LAST = LEAD_FIRST + MAX_LEAD_KEYS,
  CYCLE,
  SYSTER,
  TT_FIRST,
  TT_LAST = TT_FIRST + MAX_TT_KEYS,
  STENO_FIRST,
  STENO_LAST = STENO_FIRST + MAX_STENO_KEYS,
  SC_FIRST,
  SC_LAST,
  REDIAL,
  TURBO,
  DYNAMIC_MACRO_FIRST,
  DYNAMIC_MACRO_LAST = DYNAMIC_MACRO_FIRST + MAX_DYNAMIC_MACROS,
  OS_META_STICKY,
  OS_ACTIVE_STICKY,
  OS_CANCEL,
  CS_FIRST,
  CS_LAST = CS_FIRST + MAX_CS_KEYS,
  QK_FIRST,
  QK_LAST = QK_FIRST + MAX_QK_KEYS,

  SAFE_START,
  KALEIDOSCOPE_SAFE_START = SAFE_START
};

}  // namespace ranges
}  // namespace kaleidoscope
