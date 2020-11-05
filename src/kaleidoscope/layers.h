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

#include <Arduino.h>
#include "kaleidoscope/key_defs.h"
#include "kaleidoscope/keymaps.h"
#include "kaleidoscope/device/device.h"
#include "kaleidoscope_internal/device.h"
#include "kaleidoscope_internal/sketch_exploration/sketch_exploration.h"
#include "kaleidoscope_internal/shortname.h"
#include "kaleidoscope_internal/deprecations.h"

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

extern uint8_t layer_count;

namespace kaleidoscope {
class Layer_ {
 public:
  Layer_() {}

  void setup();

  /* There are two lookup functions, because we have two caches, and different
   * parts of the firmware will want to use either this or that (or perhaps
   * both, in rare cases).
   *
   * First of all, we use caches because looking up a key through all the layers
   * is costy, and the cost increases dramatically the more layers we have.
   *
   * Then, we have the `liveCompositeKeymap`, because to have layer behaviours
   * we want, that is, if you hold a key on a layer, release the layer key but
   * continue holding the other, we want for the layered keycode to continue
   * repeating.
   *
   * At the same time, we want other keys to not be affected by the
   * now-turned-off layer. So we update the keycode in the cache on-demand, when
   * the key is pressed. (see the top of `handleKeyswitchEvent`).
   *
   * On the other hand, we also have plugins that scan the whole keymap, and do
   * things based on that information, such as highlighting keys that changed
   * between layers. These need to be able to look at a state of where the
   * keymap *should* be, not necessarily where it is. The `liveCompositeKeymap`
   * is not useful here. So we use `activeLayers` which we update whenever
   * layers change (see `Layer.on` and `Layer.off`), and it updates the cache to
   * show how the keymap should look, without the `liveCompositeKeymap`-induced
   * behaviour.
   *
   * Thus, if we are curious about what a given key will do, use `lookup`. If we
   * are curious what the active layer state describes the key as, use
   * `lookupOnActiveLayer`.
   */
  static Key lookup(KeyAddr key_addr) {
    return live_composite_keymap_[key_addr.toInt()];
  }
  static Key lookupOnActiveLayer(KeyAddr key_addr) {
    uint8_t layer = active_layer_keymap_[key_addr.toInt()];
    return (*getKey)(layer, key_addr);
  }
  static uint8_t lookupActiveLayer(KeyAddr key_addr) {
    return active_layer_keymap_[key_addr.toInt()];
  }

  static void activate(uint8_t layer);
  static void deactivate(uint8_t layer);
  static void activateNext();
  static void deactivateTop() DEPRECATED(LAYER_DEACTIVATETOP) {
    deactivateMostRecent();
  }
  static void deactivateMostRecent();
  static void move(uint8_t layer);

  static uint8_t top(void) DEPRECATED(LAYER_TOP) {
    return mostRecent();
  }
  static uint8_t mostRecent() {
    return active_layers_[active_layer_count_ - 1];
  }
  static boolean isActive(uint8_t layer);

  static uint32_t getLayerState(void) DEPRECATED(LAYER_GETLAYERSTATE) {
    return layer_state_;
  }

  static Key eventHandler(Key mappedKey, KeyAddr key_addr, uint8_t keyState);

  typedef Key(*GetKeyFunction)(uint8_t layer, KeyAddr key_addr);
  static GetKeyFunction getKey;

  static Key getKeyFromPROGMEM(uint8_t layer, KeyAddr key_addr);

  static void updateLiveCompositeKeymap(KeyAddr key_addr, Key mappedKey) {
    live_composite_keymap_[key_addr.toInt()] = mappedKey;
  }
  static void updateLiveCompositeKeymap(KeyAddr key_addr);
  static void updateActiveLayers(void);

 private:
  using forEachHandler = void(*)(uint8_t index, uint8_t layer);

 public:
  static void forEachActiveLayer(forEachHandler h);

 private:
  static uint32_t layer_state_;
  static uint8_t active_layer_count_;
  static int8_t active_layers_[31];
  static Key live_composite_keymap_[kaleidoscope_internal::device.numKeys()];
  static uint8_t active_layer_keymap_[kaleidoscope_internal::device.numKeys()];

  static void handleKeymapKeyswitchEvent(Key keymapEntry, uint8_t keyState);
};
}

extern kaleidoscope::Layer_ Layer;
