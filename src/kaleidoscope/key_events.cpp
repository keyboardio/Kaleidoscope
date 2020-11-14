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

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/LiveKeys.h"
#include "kaleidoscope/hooks.h"
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/layers.h"

static bool handleSyntheticKeyswitchEvent(Key mappedKey, uint8_t keyState) {
  if (mappedKey.getFlags() & RESERVED)
    return false;

  if (!(mappedKey.getFlags() & SYNTHETIC))
    return false;

  using kaleidoscope::Runtime;

  if (mappedKey.getFlags() & IS_CONSUMER) {
    if (keyIsPressed(keyState))
      Runtime.hid().keyboard().pressConsumerControl(mappedKey);
  } else if (mappedKey.getFlags() & IS_SYSCTL) {
    if (keyToggledOn(keyState)) {
      Runtime.hid().keyboard().pressSystemControl(mappedKey);
    } else if (keyToggledOff(keyState)) {
      Runtime.hid().keyboard().releaseSystemControl(mappedKey);
    }
  } else if (mappedKey.getFlags() & IS_INTERNAL) {
    return false;
  } else if (mappedKey.getFlags() & SWITCH_TO_KEYMAP) {
    Layer.handleKeymapKeyswitchEvent(mappedKey, keyState);
  }

  return true;
}

static bool handleKeyswitchEventDefault(Key mappedKey, KeyAddr key_addr, uint8_t keyState) {
  using kaleidoscope::Runtime;
  //for every newly pressed button, figure out what logical key it is and send a key down event
  // for every newly released button, figure out what logical key it is and send a key up event

  if (mappedKey.getFlags() & SYNTHETIC) {
    handleSyntheticKeyswitchEvent(mappedKey, keyState);
  } else if (keyToggledOn(keyState)) {
    Runtime.hid().keyboard().pressKey(mappedKey);
  } else if (keyIsPressed(keyState)) {
    Runtime.hid().keyboard().pressKey(mappedKey, false);
  } else if (keyToggledOff(keyState) && (keyState & INJECTED)) {
    Runtime.hid().keyboard().releaseKey(mappedKey);
  }
  return true;
}

void handleKeyswitchEvent(Key mappedKey, KeyAddr key_addr, uint8_t keyState) {

  using kaleidoscope::Runtime;

  /* These first steps are only done for keypresses that have a valid key_addr.
   * In particular, doing them for keypresses with out-of-bounds key_addr
   *   would cause out-of-bounds array accesses in Layer.lookup(),
   *   Layer.updateLiveCompositeKeymap(), etc.
   * Note that many INJECTED keypresses use UnknownKeyswitchLocation
   *   which gives us an invalid key_addr here.  Therefore, it's legitimate that
   *   we may have keypresses with out-of-bounds key_addr.
   * However, some INJECTED keypresses do have valid key_addr if they are
   *   injecting an event tied to a physical keyswitch - and we want them to go
   *   through this lookup.
   * So we can't just test for INJECTED here, we need to test the key_addr
   *   directly.
   * Note also that this key_addr test avoids out-of-bounds accesses in *core*,
   *   but doesn't guarantee anything about event handlers - event handlers may
   *   still receive out-of-bounds key_addr, and handling that properly is on
   *   them.
   */
  if (key_addr.isValid()) {
    // If the caller did not supply a `Key` value, get it from the keymap
    // cache. If that value is transparent, look it up from the active layer for
    // that key address.
    if (mappedKey == Key_NoKey) {
      // Note: If the next line returns `Key_NoKey`, that will effectively mask
      // the key.
      mappedKey = Layer.lookup(key_addr);
    }

  }  // key_addr valid

  // Keypresses with out-of-bounds key_addr start here in the processing chain
  auto result = kaleidoscope::Hooks::onKeyswitchEvent(mappedKey, key_addr, keyState);

  // If any plugin returns `ABORT`, stop here and don't update the active keys
  // cache entry.
  if (result == kaleidoscope::EventHandlerResult::ABORT)
    return;

  // Update the keyboard state array
  if (key_addr.isValid()) {
    if (keyToggledOn(keyState)) {
      kaleidoscope::live_keys.activate(key_addr, mappedKey);
    } else if (keyToggledOff(keyState)) {
      kaleidoscope::live_keys.clear(key_addr);
    }
  }

  // Only continue if all plugin handlers returned `OK`.
  if (result != kaleidoscope::EventHandlerResult::OK)
    return;

  // If the key has been masked (i.e. `Key_NoKey`), or it's a plugin-specific
  // key (`RESERVED`), don't bother continuing.
  if (mappedKey == Key_NoKey || (mappedKey.getFlags() & RESERVED) != 0)
    return;

  // Handle all built-in key types.
  handleKeyswitchEventDefault(mappedKey, key_addr, keyState);
}
