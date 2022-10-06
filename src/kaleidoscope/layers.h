/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2021  Keyboard.io, Inc.
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

#include <Arduino.h>  // for PROGMEM
#include <stdint.h>   // for uint8_t, int8_t

#include "kaleidoscope/KeyAddr.h"                                         // for KeyAddr
#include "kaleidoscope/KeyEvent.h"                                        // for KeyEvent
#include "kaleidoscope/device/device.h"                                   // for Device
#include "kaleidoscope/key_defs.h"                                        // for Key
#include "kaleidoscope/keymaps.h"                                         // IWYU pragma: keep
#include "kaleidoscope/macro_helpers.h"                                   // for __NL__
#include "kaleidoscope_internal/device.h"                                 // for device
#include "kaleidoscope_internal/shortname.h"                              // for _INIT_HID_GETSH...
#include "kaleidoscope_internal/sketch_exploration/sketch_exploration.h"  // for _INIT_SKETCH_EX...
// -----------------------------------------------------------------------------
// Deprecation warning messages
#include "kaleidoscope_internal/deprecations.h"  // for DEPRECATED

#define _DEPRECATED_MESSAGE_LAYER_ACTIVATE_NEXT                             \
  "The `Layer.activateNext() function is deprecated, and will be removed\n" \
  "after 2023-04-06."

#define _DEPRECATED_MESSAGE_LAYER_DEACTIVATE_MOST_RECENT                    \
  "The `Layer.deactivateMostRecent() function is deprecated, and will be\n" \
  "removed after 2023-04-06."
// -----------------------------------------------------------------------------

// clang-format off
#ifndef MAX_ACTIVE_LAYERS
#define MAX_ACTIVE_LAYERS 16
#endif

#define START_KEYMAPS                                                   __NL__ \
   constexpr Key keymaps_linear[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns] PROGMEM = {

#define END_KEYMAPS                                                     __NL__ \
   };                                                                   __NL__ \
   uint8_t layer_count                                                  __NL__ \
      = sizeof(keymaps_linear) / sizeof(*keymaps_linear);               __NL__ \
                                                                        __NL__ \
  _INIT_SKETCH_EXPLORATION                                              __NL__ \
  _INIT_HID_GETSHORTNAME

// Macro for defining the keymap. This should be used in the sketch
// file (*.ino) to define the keymap[] array that holds the user's
// layers. It also computes the number of layers in that keymap.
#define KEYMAPS(layers...)                                              __NL__ \
  START_KEYMAPS                                                         __NL__ \
     layers                                                             __NL__ \
  END_KEYMAPS

// clang-format on

extern uint8_t layer_count;

namespace kaleidoscope {
class Layer_ {
 public:
  Layer_() {}

  void setup();

  // There are two lookup functions here, for historical reasons. Previously,
  // Kaleidoscope would need to look up a value for each active keyswitch in
  // every cycle, and pass that value on to the "event" handlers. Most of these
  // lookups were for keys that were being held, not toggled on or off. Because
  // these lookups were so frequent, a cache was used to speed them up.
  //
  // We no longer need to look up these values every cycle for keys that are
  // held, because Kaleidoscope now only acts on key events that are actual
  // toggle-on or toggle-off events, so the speed of the lookups here is not so
  // critical. However, the old "live composite keymap" cache was also used by
  // some plugins (and certain parts of Kaleidoscope itself) to override values
  // in the keymap, and these plugins might use calls to `Layer.lookup()`,
  // expecting to get the override values.
  //
  // Therefore, the `lookup()` function below first checks the `live_keys` array
  // (the keyboard state array that has replaced the keymap cache). This should
  // allow old code to continue working, until all the associated code (mostly
  // the `onKeyswitchEvent()` handlers) is replaced, at which point we can
  // remove dependence on `live_keys` entirely from this class.
  //
  // The `Runtime.lookupKey()` function replaces this one, for plugins that
  // still want to do this same check.

  static Key lookupOnActiveLayer(KeyAddr key_addr) {
    uint8_t layer = active_layer_keymap_[key_addr.toInt()];
    return (*getKey)(layer, key_addr);
  }
  static uint8_t lookupActiveLayer(KeyAddr key_addr) {
    return active_layer_keymap_[key_addr.toInt()];
  }

  static void activate(uint8_t layer);
  static void deactivate(uint8_t layer);
  DEPRECATED(LAYER_ACTIVATE_NEXT)
  static void activateNext();
  DEPRECATED(LAYER_DEACTIVATE_MOST_RECENT)
  static void deactivateMostRecent();
  static void move(uint8_t layer);

  static uint8_t mostRecent() {
    uint8_t top_layer = active_layers_[active_layer_count_ - 1];
    return unshifted(top_layer);
  }
  static bool isActive(uint8_t layer);

  static void handleLayerKeyEvent(const KeyEvent &event);

  typedef Key (*GetKeyFunction)(uint8_t layer, KeyAddr key_addr);
  static GetKeyFunction getKey;

  static Key getKeyFromPROGMEM(uint8_t layer, KeyAddr key_addr);

  static void updateActiveLayers(void);

 private:
  using forEachHandler = void (*)(uint8_t index, uint8_t layer);

 public:
  static void forEachActiveLayer(forEachHandler h);

 private:
  static uint8_t active_layer_count_;
  static int8_t active_layers_[MAX_ACTIVE_LAYERS];
  static uint8_t active_layer_keymap_[kaleidoscope_internal::device.numKeys()];

  static int8_t stackPosition(uint8_t layer);
  static void remove(uint8_t stack_index);
  static uint8_t unshifted(uint8_t layer);
};
}  // namespace kaleidoscope

extern kaleidoscope::Layer_ Layer;
