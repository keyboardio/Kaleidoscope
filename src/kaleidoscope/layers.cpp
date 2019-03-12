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

#include "kaleidoscope/Kaleidoscope.h"

// The maximum number of layers allowed. `layer_state_`, which stores
// the on/off status of the layers in a bitfield has only 32 bits, and
// that should be enough for almost any layout.
#define MAX_LAYERS sizeof(uint32_t) * 8;

// The total number of defined layers in the firmware sketch keymaps[]
// array. If the keymap wasn't defined using KEYMAPS(), set it to the
// highest possible number of layers.
uint8_t layer_count __attribute__((weak)) = MAX_LAYERS;

namespace kaleidoscope {
uint32_t Layer_::layer_state_;
uint8_t Layer_::top_active_layer_;
Key Layer_::live_composite_keymap_[ROWS][COLS];
uint8_t Layer_::active_layers_[ROWS][COLS];
Key(*Layer_::getKey)(uint8_t layer, byte row, byte col) = Layer.getKeyFromPROGMEM;

void Layer_::handleKeymapKeyswitchEvent(Key keymapEntry, uint8_t keyState) {
  if (keymapEntry.keyCode >= LAYER_SHIFT_OFFSET) {
    uint8_t target = keymapEntry.keyCode - LAYER_SHIFT_OFFSET;

    switch (target) {
    case KEYMAP_NEXT:
      if (keyToggledOn(keyState))
        activateNext();
      else if (keyToggledOff(keyState))
        deactivateTop();
      break;

    case KEYMAP_PREVIOUS:
      if (keyToggledOn(keyState))
        deactivateTop();
      else if (keyToggledOff(keyState))
        activateNext();
      break;

    default:
      /* The default case is when we are switching to a layer by its number, and
       * is a bit more complicated than switching there when the key toggles on,
       * and away when it toggles off.
       *
       * We want to handle the case where we have more than one momentary layer
       * key on our keymap that point to the same target layer, and we hold
       * both, and release one. In this case, the layer should remain active,
       * because the second momentary key is still held.
       *
       * To do this, we turn the layer back on if the switcher key is still
       * held, not only when it toggles on. So when one of them is released,
       * that does turn the layer off, but with the other still being held, the
       * layer will toggle back on in the same cycle.
       */
      if (keyIsPressed(keyState)) {
        if (!Layer.isActive(target))
          activate(target);
      } else if (keyToggledOff(keyState)) {
        deactivate(target);
      }
      break;
    }
  } else if (keyToggledOn(keyState)) {
    // switch keymap and stay there
    if (Layer.isActive(keymapEntry.keyCode) && keymapEntry.keyCode)
      deactivate(keymapEntry.keyCode);
    else
      activate(keymapEntry.keyCode);
  }
}

Key Layer_::eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | SWITCH_TO_KEYMAP))
    return mappedKey;

  handleKeymapKeyswitchEvent(mappedKey, keyState);
  return Key_NoKey;
}

Key Layer_::getKeyFromPROGMEM(uint8_t layer, byte row, byte col) {
  Key key;

  key.raw = pgm_read_word(&(keymaps[layer][row][col]));

  return key;
}

void Layer_::updateLiveCompositeKeymap(byte row, byte col) {
  int8_t layer = active_layers_[row][col];
  live_composite_keymap_[row][col] = (*getKey)(layer, row, col);
}

void Layer_::updateActiveLayers(void) {
  memset(active_layers_, 0, ROWS * COLS);
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      int8_t layer = top_active_layer_;

      while (layer > 0) {
        if (Layer.isActive(layer)) {
          Key mappedKey = (*getKey)(layer, row, col);

          if (mappedKey != Key_Transparent) {
            active_layers_[row][col] = layer;
            break;
          }
        }
        layer = nextLowerLayer(layer);
      }
    }
  }
}

void Layer_::updateTopActiveLayer(void) {
  // If layer_count is set, start there, otherwise search from the
  // highest possible layer (MAX_LAYERS) for the top active layer
  for (byte i = (layer_count - 1); i > 0; i--) {
    if (bitRead(layer_state_, i)) {
      top_active_layer_ = i;
      return;
    }
  }
  // It's not possible to turn off the default layer (see
  // updateActiveLayers()), so if no other layers are active:
  top_active_layer_ = 0;
}

void Layer_::move(uint8_t layer) {
  layer_state_ = 0;
  activate(layer);
}

// Activate a given layer
void Layer_::activate(uint8_t layer) {
  // If we're trying to turn on a layer that doesn't exist, abort (but
  // if the keymap wasn't defined using the KEYMAPS() macro, proceed anyway
  if (layer >= layer_count)
    return;

  // If the target layer was already on, return
  if (isActive(layer))
    return;

  // Otherwise, turn on its bit in layer_state_
  bitSet(layer_state_, layer);

  // If the target layer is above the previous highest active layer,
  // update top_active_layer_
  if (layer > top_active_layer_)
    updateTopActiveLayer();

  // Update the keymap cache (but not live_composite_keymap_; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

// Deactivate a given layer
void Layer_::deactivate(uint8_t layer) {
  // If the target layer was already off, return
  if (!bitRead(layer_state_, layer))
    return;

  // Turn off its bit in layer_state_
  bitClear(layer_state_, layer);

  // If the target layer was the previous highest active layer,
  // update top_active_layer_
  if (layer == top_active_layer_)
    updateTopActiveLayer();

  // Update the keymap cache (but not live_composite_keymap_; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

boolean Layer_::isActive(uint8_t layer) {
  return bitRead(layer_state_, layer);
}

void Layer_::activateNext(void) {
  activate(top_active_layer_ + 1);
}

void Layer_::deactivateTop(void) {
  deactivate(top_active_layer_);
}

__attribute__((weak))
uint8_t nextLowerLayer(uint8_t layer) {
  return layer - 1;
}

}

kaleidoscope::Layer_ Layer;
