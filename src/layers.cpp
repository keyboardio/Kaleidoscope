#include "Kaleidoscope.h"

static uint8_t DefaultLayer;
static uint32_t LayerState;

uint8_t Layer_::highestLayer;
Key Layer_::keyMap[ROWS][COLS];
Key(*Layer_::getKey)(uint8_t layer, byte row, byte col) = Layer.getKeyFromPROGMEM;

static void handleKeymapKeyswitchEvent(Key keymapEntry, uint8_t keyState) {
  if (keymapEntry.keyCode >= MOMENTARY_OFFSET) {
    uint8_t target = keymapEntry.keyCode - MOMENTARY_OFFSET;

    if (keyToggledOn(keyState)) {
      if (target == KEYMAP_NEXT) {
        Layer.next();
      } else if (target == KEYMAP_PREVIOUS) {
        Layer.previous();
      } else {
        Layer.on(target);
      }
    }
    if (keyToggledOff(keyState)) {
      if (target == KEYMAP_NEXT) {
        Layer.previous();
      } else if (target == KEYMAP_PREVIOUS) {
        Layer.next();
      } else {
        Layer.off(target);
      }
    }

    // switch keymap and stay there
  } else if (keyToggledOn(keyState)) {
    if (Layer.isOn(keymapEntry.keyCode) && keymapEntry.keyCode)
      Layer.off(keymapEntry.keyCode);
    else
      Layer.on(keymapEntry.keyCode);
  }
}

Key
Layer_::eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | SWITCH_TO_KEYMAP))
    return mappedKey;

  handleKeymapKeyswitchEvent(mappedKey, keyState);
  return Key_NoKey;
}

Layer_::Layer_(void) {
  defaultLayer(0);
}

Key
Layer_::getKeyFromPROGMEM(uint8_t layer, byte row, byte col) {
  Key key;

  key.raw = pgm_read_word(&(keymaps[layer][row][col]));

  return key;
}

void
Layer_::updateKeymapCache(byte row, byte col) {
  int8_t layer = highestLayer;

  if (row >= ROWS || col >= COLS)
    return;

  for (layer = highestLayer; layer >= DefaultLayer; layer--) {
    if (Layer.isOn(layer)) {
      Key mappedKey = (*getKey)(layer, row, col);

      if (mappedKey != Key_Transparent) {
        keyMap[row][col] = mappedKey;
        break;
      }
    }
  }
}

void
Layer_::updateKeymapCache(void) {
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      updateKeymapCache(row, col);
    }
  }
}

uint8_t Layer_::top(void) {
  for (int8_t i = 31; i >= 0; i--) {
    if (bitRead(LayerState, i))
      return i;
  }
  return 0;
}

void Layer_::move(uint8_t layer) {
  LayerState = 0;
  on(layer);
}

void Layer_::on(uint8_t layer) {
  bitSet(LayerState, layer);
  if (layer > highestLayer)
    highestLayer = layer;

  // Update the key cache, so that if anything depends on knowing the active
  // layout, the layout will be in sync.
  updateKeymapCache();
}

void Layer_::off(uint8_t layer) {
  bitClear(LayerState, layer);
  if (layer == highestLayer)
    highestLayer = top();

  // Update the key cache, so that if anything depends on knowing the active
  // layout, the layout will be in sync.
  updateKeymapCache();
}

boolean Layer_::isOn(uint8_t layer) {
  return bitRead(LayerState, layer);
}

void Layer_::next(void) {
  on(top() + 1);
}

void Layer_::previous(void) {
  off(top());
}

void Layer_::defaultLayer(uint8_t layer) {
  move(layer);
  DefaultLayer = layer;
}

uint8_t Layer_::defaultLayer(void) {
  return DefaultLayer;
}

uint32_t Layer_::getLayerState(void) {
  return LayerState;
}

Layer_ Layer;
