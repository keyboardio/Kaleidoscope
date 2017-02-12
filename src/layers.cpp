#include "layers.h"
#include "key_events.h"

static uint8_t DefaultLayer;
static uint32_t LayerState;

uint8_t Layer_::highestLayer;
uint8_t Layer_::keyMap[ROWS][COLS];

static void handle_keymap_key_event(Key keymapEntry, uint8_t keyState) {
    if (keymapEntry.keyCode >= MOMENTARY_OFFSET) {
        uint8_t target = keymapEntry.keyCode - MOMENTARY_OFFSET;

        if (key_toggled_on(keyState)) {
            if ( target == KEYMAP_NEXT) {
                Layer.next();
            } else if ( target == KEYMAP_PREVIOUS) {
                Layer.previous();
            } else {
                Layer.on(target);
            }
        }
        if (key_toggled_off(keyState)) {
            if ( target == KEYMAP_NEXT) {
                Layer.previous();
            } else if ( target == KEYMAP_PREVIOUS) {
                Layer.next();
            } else {
                Layer.off(target);
            }
        }

        // switch keymap and stay there
    } else if (key_toggled_on(keyState)) {
        if (Layer.isOn (keymapEntry.keyCode) && keymapEntry.keyCode)
            Layer.off(keymapEntry.keyCode);
        else
            Layer.on(keymapEntry.keyCode);
    }
}

Key
Layer_::eventHandler(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (mappedKey.flags != (SYNTHETIC | SWITCH_TO_KEYMAP))
        return mappedKey;

    handle_keymap_key_event(mappedKey, keyState);
    return Key_NoKey;
}

Layer_::Layer_ (void) {
    defaultLayer (0);
}

void
Layer_::mergeLayers(void) {

    memset (keyMap, DefaultLayer, ROWS * COLS);

    if (LayerState == (uint32_t)(1 << DefaultLayer))
        return;

    for (uint8_t r = 0; r < ROWS; r++) {
        for (uint8_t c = 0; c < COLS; c++) {
            int8_t layer = highestLayer;

            while (layer >= DefaultLayer) {
                if (Layer.isOn (layer)) {
                    Key mappedKey;

                    mappedKey.raw = pgm_read_word(&(keymaps[layer][r][c]));

                    if (mappedKey != Key_Transparent) {
                        keyMap[r][c] = layer;
                        break;
                    }
                }
                layer--;
            }
        }
    }
}

Key Layer_::lookup(byte row, byte col) {
    uint8_t layer = keyMap[row][col];
    Key mappedKey;
    mappedKey.raw = pgm_read_word(&(keymaps[layer][row][col]));

    return mappedKey;
}

uint8_t Layer_::top (void) {
    for (int8_t i = 31; i >= 0; i--) {
        if (bitRead (LayerState, i))
            return i;
    }
    return 0;
}

void Layer_::move (uint8_t layer) {
    LayerState = 0;
    on (layer);
}

void Layer_::on (uint8_t layer) {
    bitSet (LayerState, layer);
    if (layer > highestLayer)
        highestLayer = layer;
    mergeLayers();
}

void Layer_::off (uint8_t layer) {
    bitClear (LayerState, layer);
    if (layer == highestLayer)
        highestLayer = top();
    mergeLayers();
}

boolean Layer_::isOn (uint8_t layer) {
    return bitRead(LayerState, layer);
}

void Layer_::next (void) {
    on (top () + 1);
}

void Layer_::previous (void) {
    off (top ());
}

void Layer_::defaultLayer (uint8_t layer) {
    move (layer);
    DefaultLayer = layer;
}

uint8_t Layer_::defaultLayer (void) {
    return DefaultLayer;
}

uint32_t Layer_::getLayerState (void) {
    return LayerState;
}

Layer_ Layer;
