#include "Kaleidoscope.h"

static uint8_t DefaultLayer;
static uint32_t LayerState;

uint8_t Layer_::highestLayer;
Key Layer_::keyMap[ROWS][COLS];
Key(*Layer_::getKey)(uint8_t layer, byte row, byte col) = Layer.getKeyFromPROGMEM;
bool Layer_::repeat_first_press;

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

      /*
       * When toggling a layer off, we mask all keys still held. Masked keys
       * will be ignored until released and pressed again (see
       * `handleKeyswitchEvent` in key_events.cpp).
       *
       * We do this because when holding a momentary layer switch key, then
       * pressing and holding some others, they will fire as keys on the
       * momentary layer. But if we release the momentary layer switch key
       * before releasing the others, they will continue firing, but from
       * another layer. When typing fast, it may easily happen that we end up in
       * a situation where the layer key releases first (in the same scan cycle,
       * but handled first), and it will emit a key from the wrong layer. So we
       * ignore held keys after releasing a layer key, until they are pressed
       * again, to avoid the aforementioned issue.
       */
      if (!Layer.repeat_first_press)
        KeyboardHardware.maskHeldKeys();
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
Layer_::updateKeyCache(byte row, byte col) {
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
}

void Layer_::off(uint8_t layer) {
  bitClear(LayerState, layer);
  if (layer == highestLayer)
    highestLayer = top();
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
