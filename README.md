# Kaleidoscope-Colormap

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-Colormap.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-Colormap

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

The `Colormap` extension provides an easier way to set up a different - static -
color map per-layer. This means that we can set up a map of colors for each key,
on a per-layer basis, and whenever a layer becomes active, the color map for
that layer is applied on top of everything else. Colors are picked from a
15-color palette (or 16, if we disable transparency), provided by
the [LED-Palette-Theme][plugin:l-p-t] plugin. The color map is stored in
`EEPROM`, and can be easily changed via the [Focus][plugin:focus] plugin, which
also provides palette editing capabilities.

 [plugin:focus]: https://github.com/keyboardio/Kaleidoscope-Focus
 [plugin:l-p-t]: https://github.com/keyboardio/Kaleidoscope-LED-Palette-Theme

## Using the extension

To use the extension, include the header, tell it the number of layers you have,
register the `Focus` hooks, and it will do the rest.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Colormap.h>
#include <Kaleidoscope-Focus.h>

void setup(void) {
  Kaleidoscope.setup();
  USE_PLUGINS(&ColormapEffect, &Focus);
  
  ColormapEffect.max_layers(1);
  Focus.addHook(FOCUS_HOOK_LEDPALETTETHEME);
  Focus.addHook(FOCUS_HOOK_COLORMAP);
}
```

## Extension methods

The extension provides an `ColormapEffect` singleton object, with a single method:

### `.max_layers(max)`

> Tells the extension to reserve space in EEPROM for up to `max` layers. Can
> only be called once, any subsequent call will be a no-op.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)
* [Kaleidoscope-Focus](https://github.com/keyboardio/Kaleidoscope-Focus)
* [Kaleidoscope-LED-Palette-Theme](https://github.com/keyboardio/Kaleidoscope-LED-Palette-Theme)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-Colormap/blob/master/examples/Colormap/Colormap.ino
