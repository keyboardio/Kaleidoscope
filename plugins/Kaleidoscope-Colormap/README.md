# Colormap

The `Colormap` extension provides an easier way to set up a different - static -
color map per-layer. This means that we can set up a map of colors for each key,
on a per-layer basis, and whenever a layer becomes active, the color map for
that layer is applied. Colors are picked from a 16-color palette, provided by
the [LED-Palette-Theme][plugin:l-p-t] plugin. The color map is stored in
`EEPROM`, and can be easily changed via the [FocusSerial][plugin:focusserial]
plugin, which also provides palette editing capabilities.

 [plugin:focusserial]: Kaleidoscope-FocusSerial.md
 [plugin:l-p-t]: Kaleidoscope-LED-Palette-Theme.md

It is also possible to set up a default palette and colormap, using the
`DefaultColormap` plugin, also provided by this package. See below for its
documentation.

## Using the extension

To use the extension, include the header, tell it the number of layers you have,
register the `Focus` hooks, and it will do the rest. We'll also set up a default
for both the palette, and the colormap.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-Colormap.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-LED-Palette-Theme.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          LEDControl,
                          LEDPaletteTheme,
                          ColormapEffect,
                          DefaultColormap,
                          Focus);

PALETTE(
 /* A list of 16 cRGB colors... */
)

COLORMAPS(
 [0] = COLORMAP(
  // List of palette indexes for each key, using the same layout
  // as the `KEYMAP` macro does for keys.
 ),
 [1] = COLORMAP_STACKED(
  // List of palette indexes for each key, using the same layout
  // as the `KEYMAP_STACKED` macro does for keys.
 )
)

void setup() {
  Kaleidoscope.setup();

  ColormapEffect.max_layers(1);
  DefaultColormap.setup();
}
```

The `PALETTE` and `COLORMAPS` macros are only used for the `DefaultColormap`
plugin, `ColormapEffect` itself makes no use of them. The `PALETTE` must always
contain a full 16-color palette. `COLORMAPS` can define colormaps for as many
layers as one wishes, but the `DefaultColormap` plugin will only copy over as
many as `ColormapEffect` is configured to support.

## Plugin methods

The extension provides an `ColormapEffect` singleton object, with a single method:

### `.max_layers(max)`

> Tells the extension to reserve space in EEPROM for up to `max` layers. Can
> only be called once, any subsequent call will be a no-op.

Also provided is an optional `DefaultColormap` plugin, with two methods:

### `.setup()`

> Intended to be called from the `setup()` method of the sketch, it checks if
> the `ColormapEffect` plugin is initialized, and if not, then copies the
> palette and the colormap over from the firmware to EEPROM.

### `.install()`

> Same as `.setup()` above, but without the initialized check. Intended to be
> used when one wants to restore the colormap to factory settings.

## Focus commands

### `colormap.map`

> Without arguments, prints the color map: palette indexes for all layers.
>
> With arguments, updates the color map with new indexes. One does not need to
> give the full map, the plugin will process as many arguments as available, and
> ignore anything past the last key on the last layer (as set by the
> `.max_layers()` method).

If the `DefaultColormap` plugin is also in use, an additional focus command is
made available:

### `colormap.install`

> Copies the default colormap and palette built into the firmware into EEPROM,
> effectively performing a factory reset for both.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
* [Kaleidoscope-LED-Palette-Theme](Kaleidoscope-LED-Palette-Theme.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/LEDs/Colormap/Colormap.ino
