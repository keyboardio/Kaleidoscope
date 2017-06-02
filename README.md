# Kaleidoscope-FingerPainter

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-FingerPainter.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-FingerPainter

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

TODO

## Using the plugin

TODO

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FingerPainter.h>

void setup () {
  Kaleidoscope.setup ();
  
  USE_PLUGINS (&EEPROMSettings, &FingerPainter);

  // TODO
}
```

## Plugin methods

The plugin provides the `FingerPainter` object, which has the following methods:

**TODO**

## Dependencies

* [Kaleidoscope-EEPROM-Settings](https://github.com/keyboardio/Kaleidoscope-EEPROM-Settings)
* [Kaleidoscope-LED-Palette-Theme](https://github.com/keyboardio/Kaleidoscope-LED-Palette-Theme)
* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-FingerPainter/blob/master/examples/FingerPainter/FingerPainter.ino
