# NumPad

This is a plugin for [Kaleidoscope][fw], that adds a NumPad-specific LED
effect and applies it when the numpad layer is active.

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include "Kaleidoscope-NumPad.h"

KALEIDOSCOPE_INIT_PLUGINS(NumPad);

void setup() {
  Kaleidoscope.setup();

  NumPad.color = CRGB(0, 0, 160); // a blue color
  NumPad.lock_hue = 85; // green
}
```

##   Plugin methods

The plugin provides the `NumPad` object, with the following properties:

### `.color`

> This property sets the color that the NumPad keys are highlighted in.
>
> The default is `CRGB(160, 0, 0)`, a red color.

### `.lock_hue`

> This property sets the color hue that the NumLock LED breathes in.
>
> The default is `170`, a blue hue.

 [fw]: https://github.com/keyboardio/Kaleidoscope
