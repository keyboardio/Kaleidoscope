# Kaleidoscope-Steno

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-Steno.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-Steno

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

Stenography is a way to write in shorthand, a chorded input system that allows
very fast input (considerably higher than normal touch typing), by using
shorthand chords and a dictionary. This plugin implements the `GeminiPR`
protocol that supports a number of systems, including [Plover][plover].

 [plover]: http://www.openstenoproject.org/plover/

While Plover supports a normal QWERTY keyboard too, having a dedicated plugin
comes with important advantages:

* No need to toggle Plover on and off, because the normal keys are not taken
  over by Plover anymore.
* Easier toggling, because you only have to toggle the layer, not Plover too. If
  you switch back to a keyboard layer, without toggling Plover off, nothing
  unexpected will happen. Plover will not take over the keys.
* The `GeminiPR` protocol supports language systems other than English.

All of this comes at a cost, however: the protocol is implemented over the
virtual serial port, any plugin that wants to use that port too, will run into
conflicts with the Steno plugin. In other words, don't use it together
with [Focus][k:focus].

 [k:focus]: https://github.com/keyboardio/Kaleidoscope-Focus

## Using the plugin

To use the plugin, simply include the header in your Sketch, tell the firmware
to use the `GeminiPR` object, and place Steno keys on your keymap. It is best
illustrated with an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Steno.h>

// Somewhere in the keymap:
S(S1), S(S2), etc

void setup() {
  Kaleidoscope.use(&GeminiPR);

  Kaleidoscope.setup ();
}
```

## Keys provided by the plugin

The plugin provides a number of keys one can put on the keymap, that allow
correspond to various Steno keys. All of these must be used together with the
`S()` macro provided by the plugin, as can be seen in the example above.

The provided keys are: `FN`, `N1`, `N2`, `N3`, `N4`, `N5`, `N6`, `S1`, `S2`,
`TL`, `KL`, `PL`, `WL`, `HL`, `RL`, `A`, `O`, `ST1`, `ST2`, `RE1`, `RE2`, `PWR`,
`ST3`, `ST4`, `E`, `U`, `FR`, `RR`, `PR`, `BR`, `LR`, `GR`, `TR`, `SR`, `DR`,
`N7`, `N8`, `N9`, `NA`, `NB`, `NC`, `ZR`.

## Plugin methods and properties

The plugin provides a `GeminiPR` object, with no public methods or properties.

## Dependencies

* [Kaleidoscope-Ranges](https://github.com/keyboardio/Kaleidoscope-Ranges)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-Steno/blob/master/examples/Steno/Steno.ino
