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
uint32_t Layer_::layer_state_[kaleidoscope::max_num_key_groups] = { 0, 0, 0, 0, 0, 0 };
uint8_t Layer_::top_active_layer_[kaleidoscope::max_num_key_groups] = { 0, 0, 0, 0, 0, 0 };
Key Layer_::live_composite_keymap_[Kaleidoscope.device().numKeys()];
uint8_t Layer_::active_layers_[Kaleidoscope.device().numKeys()];
Key(*Layer_::getKey)(uint8_t layer, KeyAddr key_addr) = Layer.getKeyFromPROGMEM;

void Layer_::handleKeymapKeyswitchEvent(Key keymapEntry, uint8_t keyState) {

  uint8_t key_group_flags = decodeKeyGroupFlags(keymapEntry.getFlags());

  if (key_group_flags == 0) {
    key_group_flags = ALL_KEY_GROUPS;
  }

  if (keymapEntry.getKeyCode() >= LAYER_SHIFT_OFFSET) {
    uint8_t target = keymapEntry.getKeyCode() - LAYER_SHIFT_OFFSET;

    switch (target) {
    case KEYMAP_NEXT:
      if (keyToggledOn(keyState))
        activateNext(key_group_flags);
      else if (keyToggledOff(keyState))
        deactivateTop(key_group_flags);
      break;

    case KEYMAP_PREVIOUS:
      if (keyToggledOn(keyState))
        deactivateTop(key_group_flags);
      else if (keyToggledOff(keyState))
        activateNext(key_group_flags);
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
        if (!Layer.isActive(target, key_group_flags))
          activate(target, key_group_flags);
      } else if (keyToggledOff(keyState)) {
        deactivate(target, key_group_flags);
      }
      break;
    }
  } else if (keyToggledOn(keyState)) {
    // switch keymap and stay there
    if (Layer.isActive(keymapEntry.getKeyCode(), key_group_flags) && keymapEntry.getKeyCode())
      deactivate(keymapEntry.getKeyCode(), key_group_flags);
    else
      activate(keymapEntry.getKeyCode(), key_group_flags);
  }
}

Key Layer_::eventHandler(Key mappedKey, KeyAddr key_addr, uint8_t keyState) {
  if (!((mappedKey.getFlags() & SYNTHETIC) && (mappedKey.getFlags() & SWITCH_TO_KEYMAP)))
    return mappedKey;

  handleKeymapKeyswitchEvent(mappedKey, keyState);
  return Key_NoKey;
}

Key Layer_::getKeyFromPROGMEM(uint8_t layer, KeyAddr key_addr) {
  return keyFromKeymap(layer, key_addr);
}

void Layer_::updateLiveCompositeKeymap(KeyAddr key_addr) {
  int8_t layer = active_layers_[key_addr.toInt()];
  live_composite_keymap_[key_addr.toInt()] = (*getKey)(layer, key_addr);
}

void Layer_::updateActiveLayers(void) {
  memset(active_layers_, 0, Kaleidoscope.device().numKeys());
  for (auto key_addr : KeyAddr::all()) {

    uint8_t key_group = groupOfKey(key_addr);

    int8_t layer = top_active_layer_[key_group];

    while (layer > 0) {
      if (Layer.isActive(layer, key_group)) {
        Key mappedKey = (*getKey)(layer, key_addr);

        if (mappedKey != Key_Transparent) {
          active_layers_[key_addr.toInt()] = layer;
          break;
        }
      }
      layer--;
    }
  }
}

void Layer_::updateTopActiveLayer(uint8_t key_group) {
  // If layer_count is set, start there, otherwise search from the
  // highest possible layer (MAX_LAYERS) for the top active layer
  for (byte i = (layer_count - 1); i > 0; i--) {
    if (bitRead(layer_state_[key_group], i)) {
      top_active_layer_[key_group] = i;
      return;
    }
  }
  // It's not possible to turn off the default layer (see
  // updateActiveLayers()), so if no other layers are active:
  top_active_layer_[key_group] = 0;
}

void Layer_::move(uint8_t layer, uint8_t key_group_flags) {

  for (uint8_t key_group = 0; key_group < kaleidoscope::max_num_key_groups; ++key_group) {

    if (!isKeyGroupFlagSet(key_group_flags, key_group)) {
      continue;
    }

    layer_state_[key_group] = 0;
  }

  activate(layer, key_group_flags);
}

// Activate a given layer
void Layer_::activate(uint8_t layer, uint8_t key_group_flags) {

  bool changes_applied = false;

  for (uint8_t key_group = 0; key_group < kaleidoscope::max_num_key_groups; ++key_group) {

    if (!isKeyGroupFlagSet(key_group_flags, key_group)) {
      continue;
    }

    // If we're trying to turn on a layer that doesn't exist, abort (but
    // if the keymap wasn't defined using the KEYMAPS() macro, proceed anyway
    if (layer >= layer_count)
      return;

    // If the target layer was already on, return
    if (isActive(layer, key_group_flags))
      continue;

    changes_applied = true;

    // Otherwise, turn on its bit in layer_state_
    bitSet(layer_state_[key_group], layer);

    // If the target layer is above the previous highest active layer,
    // update top_active_layer_
    if (layer > top_active_layer_[key_group])
      updateTopActiveLayer(key_group);
  }

  if (!changes_applied) return;

  // Update the keymap cache (but not live_composite_keymap_; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

// Deactivate a given layer

void Layer_::deactivate(uint8_t layer, uint8_t key_group_flags) {

  bool changes_applied = false;

  for (uint8_t key_group = 0; key_group < kaleidoscope::max_num_key_groups; ++key_group) {

    if (!isKeyGroupFlagSet(key_group_flags, key_group)) {
      continue;
    }

    // If the target layer was already off, return
    if (!bitRead(layer_state_[key_group], layer))
      continue;

    changes_applied = true;

    // Turn off its bit in layer_state_
    bitClear(layer_state_[key_group], layer);

    // If the target layer was the previous highest active layer,
    // update top_active_layer_
    if (layer == top_active_layer_[key_group])
      updateTopActiveLayer(key_group);
  }

  if (!changes_applied) return;

  // Update the keymap cache (but not live_composite_keymap_; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();

  kaleidoscope::Hooks::onLayerChange();
}

boolean Layer_::isActive(uint8_t layer, uint8_t key_group) {
  return bitRead(layer_state_[key_group], layer);
}

void Layer_::activateNext(uint8_t key_group_flags) {

  for (uint8_t key_group = 0; key_group < kaleidoscope::max_num_key_groups; ++key_group) {

    if (!isKeyGroupFlagSet(key_group_flags, key_group)) {
      continue;
    }
    activate(top_active_layer_[key_group] + 1, key_group_flags);
  }
}

void Layer_::deactivateTop(uint8_t key_group_flags) {

  for (uint8_t key_group = 0; key_group < kaleidoscope::max_num_key_groups; ++key_group) {

    if (!isKeyGroupFlagSet(key_group_flags, key_group)) {
      continue;
    }
    deactivate(top_active_layer_[key_group]);
  }
}

}

__attribute__((weak))
uint8_t groupOfKey(KeyAddr key_addr) {
  return Kaleidoscope.device().keyScanner().isOnLeftHalf(key_addr) ? 0 : 1;
}

kaleidoscope::Layer_ Layer;
