# Colormap-Overlay

The `Colormap-Overlay` extension provides an easier way to apply color to
specific keys and layers, regardless of the active LED mode.

## Using the extension

To use the extension, include the header, tell it the number of layers you have,
register the `Focus` hooks, and it will do the rest. We'll also set up a default
for both the palette, and the colormap.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-Colormap-Overlay.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          ColormapOverlay);

void setup() {
  Kaleidoscope.setup();

  COLORMAP_OVERLAYS(
    // Make the any key red
    kaleidoscope::plugin::Overlay(0, KeyAddr(0, 9), CRGB(0xff, 0x00, 0x00)),
    // Disable leds on the palm keys
    kaleidoscope::plugin::Overlay(0, KeyAddr(3, 6), CRGB(0x00, 0x00, 0x00)),
    kaleidoscope::plugin::Overlay(0, KeyAddr(3, 9), CRGB(0x00, 0x00, 0x00)),
  )
}
```

## Plugin methods

TODO
