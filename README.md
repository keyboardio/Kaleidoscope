# Kaleidoscope-EEPROM-Colormap

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Colormap.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-EEPROM-Colormap

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

The `EEPROM-ColorMap` extension provides an easier way to set up a different -
static - color map per-layer. This means that we can set up a map of colors for
each key, on a per-layer basis, and whenever a layer becomes active, the color
map for that layer is applied on top of everything else. The extension supports
transparent colors, to make things easier.

Both the palette and the color map is stored in EEPROM, and the palette is
limited to 15 colors (with the 16th being the transparent color). The plugin can
work together with the [Focus][plugin:focus] plugin, to make it easier to update
the palette or the color map itself.

 [plugin:focus]: https://github.com/keyboardio/Kaleidoscope-Focus

## Using the extension

To use the extension, include the header, tell it the number of layers you have,
and it will do the rest.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Colormap.h>
#include <Kaleidoscope-Focus.h>


void setup (void) {

  Kaleidoscope.setup ();
  USE_PLUGINS (&EEPROMColormapEffect, &Focus);
  
  EEPROMColormapEffect.configure (1);
  Focus.addHook (FOCUS_HOOK_COLORMAP);
}
``` 

## Extension methods

The extension provides an `EEPROMColormapEffect` singleton object, with a single method:

### `.configure(maxLayers)`

> Tells the extension to reserve space in EEPROM for up to `maxLayers` layers.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-EEPROM-Colormap/blob/master/examples/EEPROM-Colormap/EEPROM-Colormap.ino
