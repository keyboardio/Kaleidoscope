#include "Kaleidoscope.h"

static uint8_t DefaultLayer;
static uint32_t LayerState;

uint8_t Layer_::highestLayer;
Key Layer_::liveCompositeKeymap[ROWS][COLS];
uint8_t Layer_::activeLayers[ROWS][COLS];
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
Layer_::updateLiveCompositeKeymap(byte row, byte col) {
  int8_t layer = activeLayers[row][col];
  liveCompositeKeymap[row][col] = (*getKey)(layer, row, col);
}

void
Layer_::updateActiveLayers(void) {
  memset(activeLayers, DefaultLayer, ROWS * COLS);
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      int8_t layer = highestLayer;

      while (layer > DefaultLayer) {
        if (Layer.isOn(layer)) {
          Key mappedKey = (*getKey)(layer, row, col);

          if (mappedKey != Key_Transparent) {
            activeLayers[row][col] = layer;
            break;
          }
        }
        layer--;
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
  bool wasOn = isOn(layer);

  bitSet(LayerState, layer);
  if (layer > highestLayer)
    highestLayer = layer;

  /* If the layer did turn on, update the keymap cache. See layers.h for an
   * explanation about the caches we have. */
  if (!wasOn)
    updateActiveLayers();
}

void Layer_::off(uint8_t layer) {
  bool wasOn = isOn(layer);

  bitClear(LayerState, layer);
  if (layer == highestLayer)
    highestLayer = top();

  /* If the layer did turn off, update the keymap cache. See layers.h for an
   * explanation about the caches we have. */
  if (wasOn)
    updateActiveLayers();
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
