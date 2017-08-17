#pragma once

#include <Arduino.h>
#include "key_defs.h"
#include KALEIDOSCOPE_HARDWARE_H

class Layer_ {
 public:
  Layer_(void);

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
   * the key is pressed or released. (see the top of `handleKeyswitchEvent`).
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
    return liveCompositeKeymap[row][col];
  }
  static Key lookupOnActiveLayer(byte row, byte col) {
    uint8_t layer = activeLayers[row][col];
    return (*getKey)(layer, row, col);
  }
  static void on(uint8_t layer);
  static void off(uint8_t layer);
  static void move(uint8_t layer);

  static uint8_t top(void) {
    return highestLayer;
  }
  static void next(void);
  static void previous(void);

  static boolean isOn(uint8_t layer);

  static void defaultLayer(uint8_t layer);
  static uint8_t defaultLayer(void);

  static uint32_t getLayerState(void);

  static Key eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState);

  static Key(*getKey)(uint8_t layer, byte row, byte col);

  static Key getKeyFromPROGMEM(uint8_t layer, byte row, byte col);

  static void updateLiveCompositeKeymap(byte row, byte col);
  static void updateActiveLayers(void);

 private:
  static void updateHighestLayer(void);

  static uint8_t highestLayer;
  static Key liveCompositeKeymap[ROWS][COLS];
  static uint8_t activeLayers[ROWS][COLS];
};

extern Layer_ Layer;
