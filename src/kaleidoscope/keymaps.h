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
#include "kaleidoscope/KeyAddr.h"
#include "kaleidoscope_internal/device.h"

extern const Key keymaps_linear[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns];

namespace kaleidoscope {

inline
Key keyFromKeymap(uint8_t layer, KeyAddr key_addr) {
  return keymaps_linear[layer][key_addr.toInt()].readFromProgmem();
}

namespace internal {

// This class mimics a three-dimensional array as formerly used to
// store keymap arrays in progmem. It wraps the new one dimentional
// keymap array.

class Keymaps2DInterface {

 public:

  class KeymapLayer {

    const Key * const keymap_layer_;

   public:

    KeymapLayer(const Key * const keymap_layer) : keymap_layer_(keymap_layer) {}

    class KeymapRow {

      const Key * const keymap_row_;

     public:
      KeymapRow(const Key * const keymap_row) : keymap_row_(keymap_row) {}

      const Key &operator[](uint8_t col) {
        return keymap_row_[col];
      }
    };

    KeymapRow operator[](uint8_t row) {
      return KeymapRow(keymap_layer_ + row * kaleidoscope_internal::device.matrix_columns);
    }
  };

  KeymapLayer operator[](uint8_t layer) {
    return KeymapLayer(keymaps_linear[layer]);
  }
};

}
}

#define _DEPRECATED_MESSAGE_2D_KEYMAP_ARRAY                                    \
  "Accessing the keymap array directly is deprecated. Please use the function "\
  "keyFromKeymap() to access keys. The keymap array will be removed in future "\
  "versions of the firmware"

DEPRECATED(2D_KEYMAP_ARRAY)
extern kaleidoscope::internal::Keymaps2DInterface keymaps;
