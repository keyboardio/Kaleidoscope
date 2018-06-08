# Kaleidoscope-USB-Quirks

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-USB-Quirks.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-USB-Quirks

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

...

## Using the plugin

After adding one-shot keys to the keymap, all one needs to do, is enable the
plugin:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-USB-Quirks.h>

KALEIDOSCOPE_INIT_PLUGINS(USBQuirks, Macros);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0) {
    USBQuirks.toggleKeyboardProtocol();
  }
  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides one object, `USBQuirks`, which provides the following method:

### `.toggleKeyboardProtocol()`

> Toggle between `Boot` and `Report` protocol by detaching, and then
> re-attaching the USB devices, and setting the `BootKeyboard` protocol
> inbetween.
>
> This is most useful when one needs to have a boot keyboard, when one's in a
> BIOS, boot loader, or early password prompt or the like, and the host does not
> explicitly request the boot protocol for one reason or the other. With this
> toggle, we can switch between the two on-demand.
