#include "Kaleidoscope.h"

static uint8_t DefaultLayer;
static uint32_t LayerState;

uint8_t Layer_::highestLayer;
Key Layer_::liveCompositeKeymap[ROWS][COLS];
uint8_t Layer_::activeLayers[ROWS][COLS];
Key(*Layer_::getKey)(uint8_t layer, byte row, byte col) = Layer.getKeyFromPROGMEM;

// The total number of defined layers in the firmware sketch keymaps[] array
uint8_t LayerCount __attribute__((weak)) = 0;

static void handleKeymapKeyswitchEvent(Key keymapEntry, uint8_t keyState) {
  if (keymapEntry.keyCode >= LAYER_SHIFT_OFFSET) {
    uint8_t target = keymapEntry.keyCode - LAYER_SHIFT_OFFSET;

    switch (target) {
    case KEYMAP_NEXT:
      if (keyToggledOn(keyState))
        Layer.next();
      else if (keyToggledOff(keyState))
        Layer.previous();
      break;

    case KEYMAP_PREVIOUS:
      if (keyToggledOn(keyState))
        Layer.previous();
      else if (keyToggledOff(keyState))
        Layer.next();
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
        if (!Layer.isOn(target))
          Layer.on(target);
      } else if (keyToggledOff(keyState)) {
        Layer.off(target);
      }
      break;
    }
  } else if (keyToggledOn(keyState)) {
    // switch keymap and stay there
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

void Layer_::updateHighestLayer(void) {
  for (int8_t i = 31; i >= 0; i--) {
    if (bitRead(LayerState, i)) {
      highestLayer = i;
      return;
    }
  }
  highestLayer = 0;
}

void Layer_::move(uint8_t layer) {
  LayerState = 0;
  on(layer);
}

void Layer_::on(uint8_t layer) {
  // If we're trying to turn on a layer that doesn't exist; abort
  if (LayerCount != 0 && layer >= LayerCount)
    return;

  bool wasOn = isOn(layer);

  bitSet(LayerState, layer);
  if (layer > highestLayer)
    updateHighestLayer();

  /* If the layer did turn on, update the keymap cache. See layers.h for an
   * explanation about the caches we have. */
  if (!wasOn)
    updateActiveLayers();
}

void Layer_::off(uint8_t layer) {
  bool wasOn = isOn(layer);

  bitClear(LayerState, layer);
  if (layer == highestLayer)
    updateHighestLayer();

  /* If the layer did turn off, update the keymap cache. See layers.h for an
   * explanation about the caches we have. */
  if (wasOn)
    updateActiveLayers();
}

boolean Layer_::isOn(uint8_t layer) {
  return bitRead(LayerState, layer);
}

void Layer_::next(void) {
  on(highestLayer + 1);
}

void Layer_::previous(void) {
  off(highestLayer);
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
