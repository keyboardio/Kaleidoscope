// Any code that appears here is added to the bottom of the preprocessed sketch file.

#include "kaleidoscope/layers.h"

namespace kaleidoscope {

// gcc 7 (Arduino > 1.8.10) has a problem with the layer dimension of
// keymap_linear, as used by keyFromKeymap(...) being undefined.
// By moving the implemenation here,
// we can be sure that the array dimensions are known and no spurious
// warnings will result.
//
Key Layer_::getKeyFromPROGMEM(uint8_t layer, KeyAddr key_addr) {
  return keyFromKeymap(layer, key_addr);
}

} // namespace kaleidoscope
