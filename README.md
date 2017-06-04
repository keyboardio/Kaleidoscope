# Kaleidoscope-LED-Palette-Theme

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Palette-Theme.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Palette-Theme

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

A common base for plugins that want to provide themes, or theme-related
capabilities, using a 15+1 color palette. In other words, this is for plugin
authors primarily.

## Using the plugin

TODO

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-LED-Palette-Theme.h>

void setup() {
  USE_PLUGINS(&EEPROMSettings, &LEDPaletteTheme);

  Kaleidoscope.setup();

  // TODO
}
```

## Plugin methods

The plugin provides the `LEDPaletteTheme` object, which has the following methods:

**TODO**

## Dependencies

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)
* [Kaleidoscope-Focus](https://github.com/keyboardio/Kaleidoscope-Focus)
* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-LED-Palette-Theme/blob/master/examples/LED-Palette-Theme/LED-Palette-Theme.ino
