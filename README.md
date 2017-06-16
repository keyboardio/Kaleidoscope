# Kaleidoscope-LEDEffect-SolidColor

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-SolidColor.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-SolidColor

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

This plugin provides tools to build LED effects that set the entire keyboard to
a single color. For show, and for backlighting purposes.

## Using the extension

To use the plugin, include the header, declare an effect using the
`LEDSolidColor` class, and tell the firmware to use the new effect:

```c++
#include <Kaleidoscope-LEDEffect-SolidColor.h>

static LEDSolidColor solidRed(160, 0, 0);

void setup() {
  Kaleidoscope.use(&solidRed);

  Kaleidoscope.setup();
}
```

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
