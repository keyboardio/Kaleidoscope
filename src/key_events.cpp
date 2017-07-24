#include "Kaleidoscope.h"

static bool handleSyntheticKeyswitchEvent(Key mappedKey, uint8_t keyState) {
  if (mappedKey.flags & RESERVED)
    return false;

  if (!(mappedKey.flags & SYNTHETIC))
    return false;

  if (mappedKey.flags & IS_INTERNAL) {
    return false;
  } else if (mappedKey.flags & IS_CONSUMER) {
    if (keyIsPressed(keyState))
      ConsumerControl.press(mappedKey.keyCode);
    else if (keyWasPressed(keyState))
      ConsumerControl.release(mappedKey.keyCode);
  } else if (mappedKey.flags & IS_SYSCTL) {
    if (keyIsPressed(keyState))
      SystemControl.press(mappedKey.keyCode);
    else if (keyWasPressed(keyState))
      SystemControl.release();
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
    press_key(mappedKey);
  } else if (keyToggledOff(keyState) && (keyState & INJECTED)) {
    release_key(mappedKey);
  }
  return true;
}

void press_key(Key mappedKey) {
  if (mappedKey.flags & SHIFT_HELD) {
    Keyboard.press(Key_LeftShift.keyCode);
  }
  if (mappedKey.flags & CTRL_HELD) {
    Keyboard.press(Key_LeftControl.keyCode);
  }
  if (mappedKey.flags & LALT_HELD) {
    Keyboard.press(Key_LeftAlt.keyCode);
  }
  if (mappedKey.flags & RALT_HELD) {
    Keyboard.press(Key_RightAlt.keyCode);
  }
  if (mappedKey.flags & GUI_HELD) {
    Keyboard.press(Key_LeftGui.keyCode);
  }
  Keyboard.press(mappedKey.keyCode);
}


void release_key(Key mappedKey) {
  if (mappedKey.flags & SHIFT_HELD) {
    Keyboard.release(Key_LeftShift.keyCode);
  }
  if (mappedKey.flags & CTRL_HELD) {
    Keyboard.release(Key_LeftControl.keyCode);
  }
  if (mappedKey.flags & LALT_HELD) {
    Keyboard.release(Key_LeftAlt.keyCode);
  }
  if (mappedKey.flags & RALT_HELD) {
    Keyboard.release(Key_RightAlt.keyCode);
  }
  if (mappedKey.flags & GUI_HELD) {
    Keyboard.release(Key_LeftGui.keyCode);
  }
  Keyboard.release(mappedKey.keyCode);
}

void handleKeyswitchEvent(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (!(keyState & INJECTED)) {
    mappedKey = Layer.lookup(row, col);
  }
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
