# LED-ActiveModColor

With this plugin, any active modifier on the keyboard will have the LED under it
highlighted. No matter how the modifier got activated (a key press, a macro,
anything else), the coloring will apply. Layer shift keys and OneShot layer keys
count as modifiers as far as the plugin is concerned.

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

It is recommended to place the activation (the `KALEIDOSCOPE_INIT_PLUGINS` parameter) of the
plugin last, so that it can reliably override any other plugins that may work
with the LEDs, and apply the highlight over those.

## Plugin properties

The plugin provides the `ActiveModColorEffect` object, which has the following
configuration methods.  These methods all take a `cRGB` object, which can be
written as `CRGB(r, g, b)`, where `r`, `g`, and `b` are all 8-bit integers
(0-255).  For example, `CRGB(50, 0, 50)` would be a purple-ish color.

### `.setHighlightColor(color)`

> Sets the color (a `cRGB` object) to use for highlighting normal modifier keys
> and layer-shift keys. Defaults to a white color.

### `.setOneShotColor(color)`

> Sets the color (a `cRGB` object) to use for highlighting active one-shot
> keys. These are the keys that will time out or deactivate when a subsequent
> key is pressed. Defaults to a yellow color.

### `.setStickyColor(color)`

> Sets the color (a `cRGB` object) to use for highlighting "sticky" one-shot
> keys. These keys will remain active until they are pressed again. Defaults to
> a red color.

## Plugin methods

The `ActiveModColorEffect` object provides the following methods:

### `.highlightNormalModifiers(bool)`

> Can be used to enable or disable the highlighting of normal modifiers. Defaults to true.

## Dependencies

* [Kaleidoscope-LEDControl](Kaleidoscope-LEDControl.md)
* [Kaleidoscope-OneShot](Kaleidoscope-OneShot.md)
* [Kaleidoscope-OneShotMetaKeys](Kaleidoscope-OneShotMetaKeys.md)

The `ActiveModColorEffect` plugin doesn't require that either OneShot or
OneShotMetaKeys plugins are registered with `KALEIDOSCOPE_INIT_PLUGINS()` in
order to work, but it does depend on their header files.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/LEDs/LED-ActiveModColor/LED-ActiveModColor.ino
