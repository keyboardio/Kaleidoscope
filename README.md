# Kaleidoscope-FingerPainter

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-FingerPainter.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-FingerPainter

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

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
* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

  [plugin:example]: https://github.com/keyboardio/Kaleidoscope-FingerPainter/blob/master/examples/FingerPainter/FingerPainter.ino

## Notice

While the code is under the GPL-3, the `FINGERPAINTER_PALETTE_PASTEL` palette
was created by [Conache][conache], and is under a [CC-BY-SA][license:cc-by-sa]
license.

 [conache]: http://www.colourlovers.com/lover/Conache/loveNote
 [license:cc-by-sa]: http://creativecommons.org/licenses/by-sa/3.0/
