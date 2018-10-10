# Kaleidoscope-LED-ActiveModColor

[![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-ActiveModColor.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-ActiveModColor

With this plugin, any active modifier on the keyboard will have the LED under it
highlighted. No matter how the modifier got activated (a key press, a macro,
anything else), the coloring will apply. Layer keys, be them layer toggles,
momentary switches, or one-shot layer keys count as modifiers as far as the
plugin is concerned.

## Using the plugin

To use the plugin, one needs to include the header, and activate the effect. It
is also possible to use a custom color instead of the white default.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-ActiveModColor.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          ActiveModColorEffect);

void setup () {
  Kaleidoscope.setup ();

  ActiveModColorEffect.highlight_color = CRGB(0x00, 0xff, 0xff);
}
```

It is recommended to place the activation (the `Kaleidoscope.use` call) of the
plugin last, so that it can reliably override any other plugins that may work
with the LEDs, and apply the highlight over those.

## Plugin properties

The plugin provides the `ActiveModColorEffect` object, which has the following
property:

### `.highlight_color`

> The color to use for highlighting the modifiers. Defaults to a white color.

### `.sticky_color`

> The color to use for highlighting one-shot modifiers when they are sticky. Defaults to a red color.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
* [Kaleidoscope-OneShot](https://github.com/keyboardio/Kaleidoscope-OneShot)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-LED-ActiveModColor/blob/master/examples/LED-ActiveModColor/LED-ActiveModColor.ino
