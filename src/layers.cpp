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

    switch (target) {
    case KEYMAP_NEXT:
      if (keyToggledOn(keyState))
        next();
      else if (keyToggledOff(keyState))
        previous();
      break;

    case KEYMAP_PREVIOUS:
      if (keyToggledOn(keyState))
        previous();
      else if (keyToggledOff(keyState))
        next();
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
      if (keyToggledOn(keyState)) {
        highestLayer = target;
        updateActiveLayers();
      } else if (keyIsPressed(keyState)) {
        if (highestLayer != target) {
          KeyboardHardware.maskKey(row, col);
        }
      } else if (keyToggledOff(keyState)) {
        updateHighestLayer();
        updateActiveLayers();
      }
      break;
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
