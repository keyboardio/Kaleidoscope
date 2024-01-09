# Colormap-Overlay

This plugin provides an easier way to apply color to specific combinations of
keys and layers, regardless of the active LED mode. Colors are picked from a
16-color palette, provided by the [LED-Palette-Theme][plugin:l-p-t] plugin. The
overlays are stored in `EEPROM`, and can be easily changed via the
[FocusSerial][plugin:focusserial] plugin, which also provides palette editing
capabilities.

 [plugin:focusserial]: Kaleidoscope-FocusSerial.md
 [plugin:l-p-t]: Kaleidoscope-LED-Palette-Theme.md

It is also possible to set up a default palette and overrides, using the
`PALETTE` macro provided by the LED-Palette-Theme package and the
`COLORMAP_OVERLAYS` provided by this package. See below for its documentation.


## Using the extension

To use the extension, include the headers and, optionally, register the `Focus`
hooks. Use the macros mentioned above to set up a default for both the palette
and colormap overlays. Note that layers and key addresses are all zero-indexed,
and key addresses rows are top to bottom and columns are left to right. For the
key coordinates refer to the relevant header file:

- [Model 01][Model01.h]
- [Model 100][Model100.h]
- [Atreus][Atreus2.h]
- [Imago][Imago.h]

 [Model01.h]: https://github.com/keyboardio/Kaleidoscope/blob/master/plugins/Kaleidoscope-Hardware-Keyboardio-Model01/src/kaleidoscope/device/keyboardio/Model01.h#L153
 [Model100.h]: https://github.com/keyboardio/Kaleidoscope/blob/master/plugins/Kaleidoscope-Hardware-Keyboardio-Model100/src/kaleidoscope/device/keyboardio/Model100.h#L175
 [Atreus2.h]: https://github.com/keyboardio/Kaleidoscope/blob/master/plugins/Kaleidoscope-Hardware-Keyboardio-Atreus/src/kaleidoscope/device/keyboardio/Atreus2.h#66
 [Imago.h]: https://github.com/keyboardio/Kaleidoscope/blob/master/plugins/Kaleidoscope-Hardware-Keyboardio-Imago/src/kaleidoscope/device/keyboardio/Imago.h#116

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-Colormap-Overlay.h>
#include <Kaleidoscope-FocusSerial.h>

PALETTE(
 /* A list of 16 cRGB colors... */
)

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          LEDControl,
                          ColormapOverlay,
                          DefaultPalette,
                          Focus);

void setup() {
  Kaleidoscope.setup();

  COLORMAP_OVERLAYS(
    // List of overlays, using kaleidoscope::plugin::Overlay
    // kaleidoscope::plugin::Overlay({layer}, {key address}, {palette index})
    // A special value `ColormapOverlay::layer_wildcard` can be used in place of
    // a layer number to apply the color overlay on all layers
  )

  ColormapOverlay.setup();
  DefaultPalette.setup();
}
```


## Plugin methods

The extension only has a single method:

### `.setup()`

> Intended to be called from the `setup()` method of the sketch, it reserves the
> required space in EEPROM.
