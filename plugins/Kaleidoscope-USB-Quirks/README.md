# USB-Quirks

USB-Quirks provides a few methods to deal with more obscure parts of the USB spec, such as changing between `Boot` and `Report` protocols. These are in a separate plugin, because these features are not part of the USB spec, and are often workarounds for various issues. See the provided methods for more information about what they're useful for.

## Using the plugin

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
