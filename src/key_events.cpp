#include "Kaleidoscope.h"


static bool handleSyntheticKeyswitchEvent(Key mappedKey, uint8_t keyState) {
  if (mappedKey.flags & RESERVED)
    return false;

  if (!(mappedKey.flags & SYNTHETIC))
    return false;

  if (mappedKey.flags & IS_CONSUMER) {
    if (keyIsPressed(keyState))
      kaleidoscope::hid::pressConsumerControl(mappedKey);
  } else if (mappedKey.flags & IS_SYSCTL) {
    if (keyIsPressed(keyState)) {
    } else if (keyWasPressed(keyState)) {
      kaleidoscope::hid::pressSystemControl(mappedKey);
      kaleidoscope::hid::releaseSystemControl(mappedKey);
    }
  } else if (mappedKey.flags & IS_INTERNAL) {
    return false;
  } else if (mappedKey.flags & SWITCH_TO_KEYMAP) {
    // Should not happen, handled elsewhere.
  }

  return true;
}

static bool handleKeyswitchEventDefault(Key mappedKey, byte row, byte col, uint8_t keyState) {
  //for every newly pressed button, figure out what logical key it is and send a key down event
  // for every newly released button, figure out what logical key it is and send a key up event

  if (mappedKey.flags & SYNTHETIC) {
    handleSyntheticKeyswitchEvent(mappedKey, keyState);
  } else if (keyIsPressed(keyState)) {
    kaleidoscope::hid::pressKey(mappedKey);
  } else if (keyToggledOff(keyState) && (keyState & INJECTED)) {
    kaleidoscope::hid::releaseKey(mappedKey);
  }
  return true;
}

void handleKeyswitchEvent(Key mappedKey, byte row, byte col, uint8_t keyState) {
  /* These first steps are only done for keypresses that have a real (row,col).
   * In particular, doing them for keypresses with out-of-bounds (row,col)
   *   would cause out-of-bounds array accesses in Layer.lookup(),
   *   Layer.updateLiveCompositeKeymap(), etc.
   * Perhaps this check should instead be for INJECTED - I'm not clear on
   *   whether keypresses with out-of-bounds (row,col) are the same as
   *   keypresses with INJECTED set, or are a superset or a subset of
   *   INJECTED keypresses.  In any case, the (row,col) test is *safe* in that
   *   it avoids out-of-bounds accesses, at least in core.  (Can't promise
   *   anything about event handlers - they may still receive out-of-bounds
   *   (row,col), and handling that properly is on them.)
   */
  if (row < ROWS && col < COLS) {

    /* If a key had an on or off event, we update the live composite keymap. See
     * layers.h for an explanation about the different caches we have. */
    if (keyToggledOn(keyState) || keyToggledOff(keyState))
      Layer.updateLiveCompositeKeymap(row, col);

    /* If the key we are dealing with is masked, ignore it until it is released.
     * When releasing it, clear the mask, so future key events can be handled
     * appropriately.
     *
     * See layers.cpp for an example that masks keys, and the reason why it does
     * so.
     */
    if (KeyboardHardware.isKeyMasked(row, col)) {
      if (keyToggledOff(keyState)) {
        KeyboardHardware.unMaskKey(row, col);
      } else {
        return;
      }
    }

    if (!(keyState & INJECTED)) {
      mappedKey = Layer.lookup(row, col);
    }

  }  // row < ROWS && col < COLS

  // Keypresses with out-of-bounds (row,col) start here in the processing chain

  for (byte i = 0; Kaleidoscope.eventHandlers[i] != NULL && i < HOOK_MAX; i++) {
    Kaleidoscope_::eventHandlerHook handler = Kaleidoscope.eventHandlers[i];
    mappedKey = (*handler)(mappedKey, row, col, keyState);
    if (mappedKey.raw == Key_NoKey.raw)
      return;
  }
  mappedKey = Layer.eventHandler(mappedKey, row, col, keyState);
  if (mappedKey.raw == Key_NoKey.raw)
    return;
  handleKeyswitchEventDefault(mappedKey, row, col, keyState);
}
