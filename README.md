# Kaleidoscope-Escape-OneShot

![status][st:stable] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-Escape-OneShot.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-Escape-OneShot

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

Turn the `Esc` key into a special key, that can cancel any active `OneShot`
effect - or act as the normal `Esc` key if none are active. For those times when
one accidentally presses a one-shot key, or change their minds.

## Using the plugin

To use the plugin, one needs to include the header, and activate it. No further
configuration is necessary.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-Escape-OneShot.h>

void setup () {
  Kaleidoscope.setup (KEYMAP_SIZE);

  Kaleidoscope.use (&OneShot, &EscapeOneShot, NULL);
}
```

The plugin only makes sense when using one-shot keys.

## Plugin methods

The plugin provides the `EscapeOneShot` object, which has no public methods.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-Escape-OneShot/blob/master/examples/Escape-OneShot/Escape-OneShot.ino
