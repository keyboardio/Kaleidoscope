#include "Kaleidoscope.h"

static uint8_t DefaultLayer;
static uint32_t LayerState;

// The maximum number of layers allowed. `LayerState`, which stores
// the on/off status of the layers in a bitfield has only 32 bits, and
// that should be enough for almost any layout.
#define MAX_LAYERS sizeof(LayerState) * 8;

uint8_t Layer_::highestLayer;
bool Layer_::shiftOn = false;
Key Layer_::liveCompositeKeymap[ROWS][COLS];
uint8_t Layer_::activeLayers[ROWS][COLS];
Key(*Layer_::getKey)(uint8_t layer, byte row, byte col) = Layer.getKeyFromPROGMEM;

// The total number of defined layers in the firmware sketch keymaps[]
// array. If the keymap wasn't defined using KEYMAPS(), set it to the
// highest possible number of layers.
uint8_t layer_count __attribute__((weak)) = MAX_LAYERS;

void Layer_::handleKeymapKeyswitchEvent(Key keymapEntry, byte row, byte col, uint8_t keyState) {
  if (keymapEntry.keyCode >= LAYER_SHIFT_OFFSET) {
    uint8_t target = keymapEntry.keyCode - LAYER_SHIFT_OFFSET;

    // If we're using Key_KeymapNext_Momentary or Key_KeymapPrevious_Momentary, set the
    // target layer relative to the current highestLayer
    if (target == KEYMAP_NEXT) {
      target = highestLayer + 1;
    } else if (target == KEYMAP_PREVIOUS) {
      target = highestLayer - 1;
    }

    if (keyToggledOn(keyState)) {
      // When a layer shift key is pressed, shift to the target layer by updating
      // highestLayer and the active layers array. This will mask layers above
      // highestLayer, even if those layers are active.
      highestLayer = target;
      updateActiveLayers();
    } else if (keyIsPressed(keyState)) {
      // If another layer shift happened while the key was still pressed, that shift takes
      // precedence, and this key should be ignored until it's pressed again.
      if (highestLayer != target) {
        KeyboardHardware.maskKey(row, col);
      }
    } else if (keyToggledOff(keyState)) {
      // When the layer shift is released, restore the normal state of the layer stack.
      updateHighestLayer();
      updateActiveLayers();
    }

  } else if (keyToggledOn(keyState)) {
    // switch keymap and stay there
    if (isOn(keymapEntry.keyCode) && keymapEntry.keyCode)
      off(keymapEntry.keyCode);
    else
      on(keymapEntry.keyCode);
  }
}

Key
Layer_::eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | SWITCH_TO_KEYMAP))
    return mappedKey;

  handleKeymapKeyswitchEvent(mappedKey, row, col, keyState);
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
        if (isOn(layer) || layer == highestLayer) {
          Key mappedKey = (*getKey)(layer, row, col);

          if (mappedKey != Key_Transparent) {
            activeLayers[row][col] = layer;
            break;
          }
        }
        --layer;
      }
    }
  }
}

void Layer_::updateHighestLayer(void) {
  // If layer_count is set, start there, otherwise search from the
  // highest possible layer (MAX_LAYERS) for the top active layer
  for (byte i = (layer_count - 1); i > DefaultLayer; i--) {
    if (bitRead(LayerState, i)) {
      highestLayer = i;
      return;
    }
  }
  // It's not possible to turn off the default layer (see
  // updateActiveLayers()), so if no other layers are active:
  highestLayer = DefaultLayer;
}

void Layer_::move(uint8_t layer) {
  LayerState = 0;
  on(layer);
}

// Activate a given layer
void Layer_::on(uint8_t layer) {
  // If we're trying to turn on a layer that doesn't exist, abort (but
  // if the keymap wasn't defined using the KEYMAPS() macro, proceed anyway
  if (layer >= layer_count)
    return;

  // If the target layer was already on, return
  if (isOn(layer))
    return;

  // Otherwise, turn on its bit in LayerState
  bitSet(LayerState, layer);

  // If the target layer is above the previous highest active layer,
  // update highestLayer
  if (!shiftOn && layer > highestLayer)
    updateHighestLayer();

  // Update the keymap cache (but not liveCompositeKeymap; that gets
  // updated separately, when keys toggle on or off. See layers.h)
  updateActiveLayers();
}

// Deactivate a given layer
void Layer_::off(uint8_t layer) {
  // If the target layer was already off, return
  if (!bitRead(LayerState, layer))
    return;

  // Turn off its bit in LayerState
  bitClear(LayerState, layer);

  // If the target layer was the previous highest active layer,
  // update highestLayer
  if (! shiftOn && layer == highestLayer)
    updateHighestLayer();

  // Update the keymap cache (but not liveCompositeKeymap; that gets
  // updated separately, when keys toggle on or off. See layers.h)
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
