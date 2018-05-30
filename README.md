# Kaleidoscope-LEDEffect-Rainbow

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-Rainbow.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LEDEffect-Rainbow

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

Two colorful rainbow effects are implemented by this plugin: one where the
rainbow waves through the keys, and another where the LEDs breathe though the
colors of a rainbow. The difference is that in the first case, we have all the
rainbow colors on display, and it waves through the keyboard. In the second
case, we have only one color at a time, for the whole board, and the color
cycles through the rainbow's palette.

## Using the extension

To use the plugin, include the header, and tell the firmware to use either (or
both!) of the effects:

```c++
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-Rainbow.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDRainbowEffect, LEDRainbowWaveEffect);

void setup() {
  Kaleidoscope.setup();

  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);
  LEDRainbowWaveEffect.update_delay(50);
}
```

## Plugin methods

The plugin provides two objects: `LEDRainbowEffect`, and `LEDRainbowWaveEffect`,
both of which provide the following methods:

### `.brightness(brightness)`

Sets the LED brightness for the effect.

### `.update_delay(delay)`

Sets the number of miliseconds between effect updates.
Smaller number = faster rainbows.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
