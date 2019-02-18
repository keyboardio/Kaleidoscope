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
#include KALEIDOSCOPE_HARDWARE_H

extern const Key keymaps[][ROWS][COLS];


// Macro for defining the keymap. This should be used in the sketch
// file (*.ino) to define the keymap[] array that holds the user's
// layers. It also computes the number of layers in that keymap.
#define KEYMAPS(layers...)				\
  const Key keymaps[][ROWS][COLS] PROGMEM = { layers };		\
  uint8_t layer_count = sizeof(keymaps) / sizeof(*keymaps);

extern uint8_t layer_count;

namespace kaleidoscope {
class Layer_ {
 public:
  Layer_() {}

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
  static Key lookup(byte row, byte col) {
    return live_composite_keymap_[row][col];
  }
  static Key lookupOnActiveLayer(byte row, byte col) {
    uint8_t layer = active_layers_[row][col];
    return (*getKey)(layer, row, col);
  }
  static uint8_t lookupActiveLayer(byte row, byte col) {
    return active_layers_[row][col];
  }

  static void activate(uint8_t layer);
  static void deactivate(uint8_t layer);
  static void activateNext();
  static void deactivateTop();
  static void move(uint8_t layer);

  static uint8_t top(void) {
    return top_active_layer_;
  }
  static boolean isActive(uint8_t layer);

  static uint32_t getLayerState(void) {
    return layer_state_;
  }

  static Key eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState);

  static Key(*getKey)(uint8_t layer, byte row, byte col);

  static Key getKeyFromPROGMEM(uint8_t layer, byte row, byte col);

  static void updateLiveCompositeKeymap(byte row, byte col);
  static void updateActiveLayers(void);

 private:
  static uint32_t layer_state_;
  static uint8_t top_active_layer_;
  static Key live_composite_keymap_[ROWS][COLS];
  static uint8_t active_layers_[ROWS][COLS];

  static void handleKeymapKeyswitchEvent(Key keymapEntry, uint8_t keyState);
  static void updateTopActiveLayer(void);
};
}

extern kaleidoscope::Layer_ Layer;
