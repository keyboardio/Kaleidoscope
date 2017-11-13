#pragma once

// Calculate the number of layers defined in the "keymaps[]"
// array. This needs to be included in the sketch after "keymaps" is
// defined
const uint8_t LayerCount PROGMEM = sizeof(keymaps) / sizeof(*keymaps);
