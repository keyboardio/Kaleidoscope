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

#include "kaleidoscope_internal/device.h"
#include "kaleidoscope/hooks.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/KeyEvent.h"
#include "kaleidoscope/LiveKeys.h"

// The maximum number of layers allowed.
#define MAX_LAYERS 32;

// The following definitions of layer_count and keymaps_linear
// are used if the user does not define a keymap within the sketch
// by means of the KEYMAP macro.
// This can e.g. be the case if some sort of plugin is used that features
// a different layer system and registeres it with the layer class.
// In that case the layer lookup through keymaps_linear is completely
// disabled but the weak versions must be there to make the linker happy.
//
__attribute__((weak))
uint8_t layer_count = 0;

__attribute__((weak))
extern constexpr Key keymaps_linear[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns] = {};

namespace kaleidoscope {
uint8_t Layer_::active_layer_count_ = 1;
int8_t Layer_::active_layers_[31];

uint8_t Layer_::active_layer_keymap_[kaleidoscope_internal::device.numKeys()];
Layer_::GetKeyFunction Layer_::getKey = &Layer_::getKeyFromPROGMEM;

void Layer_::setup() {
  // Update the active layer cache (every entry will be `0` to start)
  Layer.updateActiveLayers();
}

void Layer_::handleLayerKeyEvent(const KeyEvent &event) {
  // The caller is responsible for checking that this is a Layer `Key`, so we
  // skip checking for it here.
  uint8_t key_code = event.key.getKeyCode();
  uint8_t target_layer;

  if (key_code >= LAYER_MOVE_OFFSET) {
    // MoveToLayer()
    if (keyToggledOn(event.state)) {
      target_layer = key_code - LAYER_MOVE_OFFSET;
      move(target_layer);
    }
  } else if (key_code >= LAYER_SHIFT_OFFSET) {
    // layer shift keys (two types)
    target_layer = key_code - LAYER_SHIFT_OFFSET;

    switch (target_layer) {
    case KEYMAP_NEXT:
      // Key_KeymapNext_Momentary
      if (keyToggledOn(event.state))
        activateNext();
      else
        deactivateMostRecent();
      break;

    case KEYMAP_PREVIOUS:
      // Key_KeymapPrevious_Momentary
      if (keyToggledOn(event.state))
        deactivateMostRecent();
      else
        activateNext();
      break;

    default:
      // ShiftToLayer()
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
      if (keyToggledOn(event.state)) {
        // Re-think this: maybe we want to bring an already-active layer to the
        // top when a layer shift key is pressed.
        if (!isActive(target_layer))
          activate(target_layer);
      } else {
        // If there's another layer shift key keeping the target layer active,
        // we need to abort before deactivating it.
        for (Key key : live_keys.all()) {
          if (key == event.key) {
            return;
          }
        }
        // No other layer shift key for the target layer is pressed; deactivate
        // it now.
        deactivate(target_layer);
      }
      break;
    }
  } else if (keyToggledOn(event.state)) {
    // LockLayer()/UnlockLayer()
    target_layer = key_code;
    // switch keymap and stay there
    if (isActive(target_layer))
      deactivate(target_layer);
    else
      activate(target_layer);
  }
}

#ifndef NDEPRECATED
void Layer_::handleKeymapKeyswitchEvent(Key key, uint8_t key_state) {
  if (key.getFlags() == (SYNTHETIC | SWITCH_TO_KEYMAP))
    handleLayerKeyEvent(KeyEvent(KeyAddr::none(), key_state, key));
}

Key Layer_::eventHandler(Key mappedKey, KeyAddr key_addr, uint8_t keyState) {
  if (mappedKey.getFlags() == (SYNTHETIC | SWITCH_TO_KEYMAP))
    handleLayerKeyEvent(KeyEvent(key_addr, keyState, mappedKey));
  return mappedKey;
}
#endif

Key Layer_::getKeyFromPROGMEM(uint8_t layer, KeyAddr key_addr) {
  return keyFromKeymap(layer, key_addr);
}

void Layer_::updateActiveLayers(void) {
  // First, set every entry in the active layer keymap to point to the default
  // layer (layer 0).
  memset(active_layer_keymap_, 0, kaleidoscope_internal::device.numKeys());

  // For each key address, set its entry in the active layer keymap to the value
  // of the top active layer that has a non-transparent entry for that address.
  for (auto key_addr : KeyAddr::all()) {
    for (uint8_t i = active_layer_count_; i > 0; --i) {
      uint8_t layer = active_layers_[i - 1];
      Key key = (*getKey)(layer, key_addr);

      if (key != Key_Transparent) {
        active_layer_keymap_[key_addr.toInt()] = layer;
        break;
      }
    }
  }
  // Even if there are no active layers (a situation that should be prevented by
  // `deactivate()`), each key will be mapped from the base layer (layer
  // 0). Likewise, for any address where all active layers have a transparent
  // entry, that key will be mapped from the base layer, even if the base layer
  // has been deactivated.
}

void Layer_::move(uint8_t layer) {
  // We do pretty much what activate() does, except we do everything
  // unconditionally, to make sure all parts of the firmware are aware of the
  // layer change.
  if (layer >= layer_count) {
    layer = 0;
  }
  active_layer_count_ = 1;
  active_layers_[0] = layer;

  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
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

  // Otherwise, push it onto the active layer stack
  active_layers_[active_layer_count_++] = layer;

  // Update the keymap cache (but not live_composite_keymap_; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

// Deactivate a given layer
void Layer_::deactivate(uint8_t layer) {
  // If the target layer was already off, return
  if (!isActive(layer))
    return;

  // If the sole active layer is being deactivated, turn on the base layer and
  // return so we always have at least one layer active.
  if (active_layer_count_ <= 1) {
    move(0);
    return;
  }

  // Remove the target layer from the active layer stack, and shift any layers
  // above it down to fill in the gap
  for (uint8_t i = 0; i < active_layer_count_; ++i) {
    if (active_layers_[i] == layer) {
      memmove(&active_layers_[i], &active_layers_[i + 1], active_layer_count_ - i);
      --active_layer_count_;
      break;
    }
  }

  // Update the keymap cache (but not live_composite_keymap_; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

boolean Layer_::isActive(uint8_t layer) {
  for (int8_t i = 0; i < active_layer_count_; ++i) {
    if (active_layers_[i] == layer)
      return true;
  }
  return false;
}

void Layer_::activateNext(void) {
  activate(active_layers_[active_layer_count_ - 1] + 1);
}

void Layer_::deactivateMostRecent(void) {
  deactivate(active_layers_[active_layer_count_ - 1]);
}

void Layer_::forEachActiveLayer(forEachHandler h) {
  for (uint8_t i = 0; i < active_layer_count_; i++) {
    (*h)(i, active_layers_[i]);
  }
}

}

kaleidoscope::Layer_ Layer;
