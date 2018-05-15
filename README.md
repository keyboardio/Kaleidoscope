# Kaleidoscope-LEDEffect-Breathe

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-Breathe.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-Breathe

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

Provides a breathing effect for the keyboard. Breathe in, breathe out.

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-Breathe.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          LEDBreatheEffect);

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides the `LEDBreatheEffect` object, which has no public methods or
properties, outside of those provided by all LED modes.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
