# Kaleidoscope-LED-Wavepool

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Wavepool.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-Wavepool

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

The `WavepoolEffect` plugin makes waves of light splash out from each keypress.
When idle, it will also simulate gentle rainfall on the keyboard.

## Using the plugin

To use the plugin, one needs to include the header and select the effect.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LED-Wavepool.h>

void setup (){
  Kaleidoscope.use(&WavepoolEffect);

  Kaleidoscope.setup();

  WavepoolEffect.activate();
}
```

It is recommended to place the activation of the plugin (the `USE_PLUGINS` call)
as early as possible, so the plugin can catch all relevant key presses.

## Plugin methods

The plugin provides the `WavepoolEffect` object, which has the following
properties:

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-LED-Wavepool/blob/master/examples/LED-Wavepool/LED-Wavepool.ino
