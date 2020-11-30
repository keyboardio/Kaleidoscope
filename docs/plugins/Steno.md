# Steno

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

Do note that the `GeminiPR` protocol is implemented over the virtual serial
port, so any plugin that wants to use that port too, will run into
conflicts with the Steno plugin. In other words, don't use it together
with [Focus][k:focus].

 [k:focus]: FocusSerial.md

## What is Steno? Why should I use it? How do I learn?

As mentioned above, steno (short for "stenography") is a shorthand, chorded
input system that allows very fast input - licensed stenographers are required
to type **225 WPM at 95% accuracy** to get their license.  Although reaching that
speed typically takes 2-6 years of practice and training, lower speeds
comparable to or exceeding that of touch typing can reportedly be reached in
only a few months.

[This talk][stenotalk] (YouTube link) gives a brief introduction to Steno, how
it works, and why it is cool.

  [stenotalk]: https://youtu.be/Wpv-Qb-dB6g

One recommend way to get started with learning Steno is with [Plover][plover].
Plover is software for your computer that will interpret the steno input from
your Model 01 (or other NKRO QWERTY keyboard); it is available for Windows,
macOS, and Linux.  Plover's [Beginner's Guide][ploverguide] is a great place to
get started with Steno in general and Plover in particular.

  [ploverguide]: https://github.com/openstenoproject/plover/wiki/Beginner's-Guide:-Get-Started-with-Plover

## Using the plugin

To use the plugin, simply include the header in your Sketch, tell the firmware
to use the `GeminiPR` object, and place Steno keys on your keymap. It is best
illustrated with an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Steno.h>

// Somewhere in the keymap:
S(S1), S(S2), etc

KALEIDOSCOPE_INIT_PLUGINS(GeminiPR);

void setup() {
  Kaleidoscope.setup();
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

See the [example][plugin:example] for the default/suggested placements of each
of these keys.

## Plugin methods and properties

The plugin provides a `GeminiPR` object, with no public methods or properties.

## Dependencies

* [Kaleidoscope-Ranges](Ranges.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Features/Steno/Steno.ino
