# Colormap-Overlay

The `Colormap-Overlay` extension provides an easier way to apply color to
specific keys and layers, regardless of the active LED mode. Colors are picked
from a 16-color palette, provided by the [LED-Palette-Theme][plugin:l-p-t]
plugin. The color map is stored in `EEPROM`, and can be easily changed via the
[FocusSerial][plugin:focusserial] plugin, which also provides palette editing
capabilities.

 [plugin:focusserial]: Kaleidoscope-FocusSerial.md
 [plugin:l-p-t]: Kaleidoscope-LED-Palette-Theme.md

It is also possible to set up a default palette and colormap, using the
`DefaultColormap` plugin, provided by the [Colormap][plugin:colormap] package.

 [plugin:colormap]: Kaleidoscope-Colormap.md

## Using the extension

To use the extension, include the header, tell it the number of layers you have,
register the `Focus` hooks, and it will do the rest. We'll also set up a default
for both the palette, and the colormap.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-Colormap.h>  // DefaultColorMap
#include <Kaleidoscope-Colormap-Overlay.h>

PALETTE(
 /* A list of 16 cRGB colors... */
)

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          LEDControl,
                          ColormapOverlay,
                          DefaultColormap);

void setup() {
  Kaleidoscope.setup();

  COLORMAP_OVERLAYS(
    // List of overlays, using kaleidoscope::plugin::Overlay
    // kaleidoscope::plugin::Overlay({layer}, {key address}, {palette index})
  )

  ColormapOverlay.setup();
  DefaultColormap.setup();
}
```

## Plugin methods

The extension only has a single method:

### `.setup()`

> Intended to be called from the `setup()` method of the sketch, it reserves the
> required space in EEPROM.
