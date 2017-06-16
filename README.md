# Kaleidoscope-LEDEffect-Chase

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-Chase.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-Chase

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

A simple LED effect where one color chases another across the keyboard and back,
over and over again. Playful colors they are.

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include <Kaleidoscope-LEDEffect-Chase.h>

void setup() {
  Kaleidoscope.use(&LEDChaseEffect);

  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides the `LEDChaseEffect` object, which has no public methods or
properties, outside of those provided by all LED modes.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
