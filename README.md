# Kaleidoscope-Heatmap

![status][st:experimental]

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

The `Heatmap` plugin provides a LED effect, that displays a heatmap on the
keyboard. The LEDs under each key will have a color according to how much use
they see. Fewer used keys will have deep blue colors, that gradually turns
lighter, then green, to yellow, and finally red for the most used keys. The
heatmap is not updated on every key press, but periodically. It's precision is
also an approximation, and not a hundred percent exact. Nevertheless, it is a
reasonable estimate.

## Using the plugin

The plugin comes with reasonable defaults pre-configured, all one needs to do is
include the header, and make sure the plugin is in use:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Heatmap.h>

void setup (void) {
  Kaleidoscope.setup (KEYMAP_SIZE);
  Kaleidoscope.use (&HeatmapEffect, NULL);
}
```

This sets up the heatmap to update every 500 cycles, which is about 2.5 seconds,
and is the default. It also registers a new LED effect, which means that if you
have not set up any other effects, then Heatmap will likely be the default. You
may not want that, so setting up at least one other LED effect, such as `LEDOff`
is highly recommended.

## Plugin methods

The plugin provides two methods on the `HeatmapEffect` object:

### `.configure(updateFrequency)`

> Sets up the update frequency of the heatmap, to update every `updateFrequency`
> milliseconds. Updating the heatmap incurs a significant performance penalty,
> and should not be done too often. Doing it too rarely, on the other hand, make
> it much less useful. One has to strike a reasonable balance.
>
> Defaults to *500*.

### `.activate()`

> When called, immediately activates the Heatmap effect. Mostly useful in the
> `setup()` method of the Sketch, or in macros that are meant to switch to the
> heatmap effect, no matter where we are in the list.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-Heatmap/blob/master/examples/Heatmap/Heatmap.ino
