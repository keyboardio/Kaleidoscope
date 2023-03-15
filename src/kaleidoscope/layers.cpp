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

#include <stdint.h>  // for uint8_t, int8_t
#include <string.h>  // for memmove, memset

#include "kaleidoscope/KeyAddr.h"          // for MatrixAddr, MatrixAddr<>::Range, KeyAddr
#include "kaleidoscope/KeyAddrMap.h"       // for KeyAddrMap<>::Iterator, KeyAddrMap
#include "kaleidoscope/KeyEvent.h"         // for KeyEvent
#include "kaleidoscope/KeyMap.h"           // for KeyMap
#include "kaleidoscope/LiveKeys.h"         // for LiveKeys, live_keys
#include "kaleidoscope/device/device.h"    // for Device
#include "kaleidoscope/hooks.h"            // for Hooks
#include "kaleidoscope/key_defs.h"         // for Key, LAYER_MOVE_OFFSET, LAYER_SHIFT_OFFSET
#include "kaleidoscope/keymaps.h"          // for keyFromKeymap
#include "kaleidoscope/keyswitch_state.h"  // for keyToggledOn
#include "kaleidoscope/layers.h"           // for Layer_, Layer, Layer_::GetKeyFunction, Layer_:...
#include "kaleidoscope_internal/device.h"  // for device

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

__attribute__((weak)) extern constexpr Key keymaps_linear[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns] = {};

namespace kaleidoscope {
uint8_t Layer_::active_layer_count_ = 1;
int8_t Layer_::active_layers_[MAX_ACTIVE_LAYERS];

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

  // If this is a ModLayer key, we need to convert it into a layer shift first.
  if (event.key.isModLayerKey())
    key_code = (key_code / 8) + LAYER_SHIFT_OFFSET;

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
    case KEYMAP_PREVIOUS:
      if (keyToggledOn(event.state)) {
        uint8_t top_layer = unshifted(active_layers_[active_layer_count_ - 1]);
        if (target_layer == KEYMAP_NEXT) {
          // Key_KeymapNext_Momentary
          target_layer = top_layer + 1;
        } else {
          // Key_KeymapPrevious_Momentary
          target_layer = top_layer - 1;
        }
        if (target_layer >= layer_count) {
          live_keys.mask(event.addr);
          return;
        }
        uint8_t target_layer_shifted = target_layer + LAYER_SHIFT_OFFSET;
        activate(target_layer_shifted);
        // We can't just change `event.key` here because `live_keys[]` has
        // already been updated by the time `handleLayerKeyEvent()` gets called.
        live_keys[event.addr] = ShiftToLayer(target_layer);
      }
      break;

    default:
      // ShiftToLayer()
      target_layer += LAYER_SHIFT_OFFSET;

      if (keyToggledOn(event.state)) {
        if (stackPosition(target_layer) < 0) {
          activate(target_layer);
        }
      } else {
        deactivate(target_layer);
      }
      break;
    }
  } else if (keyToggledOn(event.state)) {
    // LockLayer()/UnlockLayer()
    target_layer = key_code;

    // First, we find the top non-shifted layer in the stack:
    int8_t top_locked_layer = -1;
    for (uint8_t i = 0; i < active_layer_count_; ++i) {
      if (active_layers_[i] < LAYER_SHIFT_OFFSET) {
        top_locked_layer = active_layers_[i];
      }
    }

    // If the top locked layer is the target layer, we remove it from the stack.
    // Otherwise, we activate it.  We disregard shifted layers so that it's
    // possible to set up a layer toggle key on a shifted layer that will
    // actually deactivate the target layer as expected, with a single tap.
    if (top_locked_layer == target_layer) {
      deactivate(target_layer);
    } else {
      activate(target_layer);
    }
  }
}

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
      uint8_t layer = unshifted(active_layers_[i - 1]);

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
  if (layer >= layer_count)
    return;

  active_layer_count_ = 1;
  active_layers_[0]   = layer;

  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

// Activate a given layer
void Layer_::activate(uint8_t layer) {
  // If we're trying to turn on a layer that doesn't exist, abort (but
  // if the keymap wasn't defined using the KEYMAPS() macro, proceed anyway
  uint8_t layer_unshifted = unshifted(layer);

  if (layer_unshifted >= layer_count)
    return;

  int8_t old_pos = stackPosition(layer);
  if (old_pos >= 0) {
    remove(old_pos);
  }

  // Guarantee that we don't overflow by removing layers from the bottom if
  // we're about to exceed the size of the active layers array.
  while (active_layer_count_ >= MAX_ACTIVE_LAYERS) {
    remove(0);
  }

  // Otherwise, push it onto the active layer stack
  active_layers_[active_layer_count_++] = layer;

  // Update the keymap cache (but not live_composite_keymap_; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

void Layer_::remove(uint8_t i) {
  memmove(&active_layers_[i], &active_layers_[i + 1], active_layer_count_ - (i + 1));
  --active_layer_count_;
}

// Deactivate a given layer
void Layer_::deactivate(uint8_t layer) {
  int8_t current_pos = stackPosition(layer);

  // If the target layer was already off, return
  if (current_pos < 0)
    return;

  // If the sole active layer is being deactivated, turn on the base layer and
  // return so we always have at least one layer active.
  if (active_layer_count_ <= 1) {
    move(0);
    return;
  }

  // Remove the target layer from the active layer stack, and shift any layers
  // above it down to fill in the gap.
  remove(current_pos);

  // Update the keymap cache.
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

boolean Layer_::isActive(uint8_t layer) {
  return ((stackPosition(layer) >= 0) ||
          (stackPosition(layer + LAYER_SHIFT_OFFSET) >= 0));
}

int8_t Layer_::stackPosition(uint8_t layer) {
  for (int8_t i = 0; i < active_layer_count_; ++i) {
    if (active_layers_[i] == layer)
      return i;
  }
  return -1;
}

void Layer_::activateNext() {
  activate(active_layers_[active_layer_count_ - 1] + 1);
}

void Layer_::deactivateMostRecent() {
  uint8_t layer = active_layers_[active_layer_count_ - 1];
  deactivate(layer);
}

void Layer_::forEachActiveLayer(forEachHandler h) {
  for (uint8_t i = 0; i < active_layer_count_; i++) {
    uint8_t layer = unshifted(active_layers_[i]);
    (*h)(i, layer);
  }
}

uint8_t Layer_::unshifted(uint8_t layer) {
  if (layer >= LAYER_SHIFT_OFFSET)
    layer -= LAYER_SHIFT_OFFSET;
  return layer;
}

}  // namespace kaleidoscope

kaleidoscope::Layer_ Layer;
