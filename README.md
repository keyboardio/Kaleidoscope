# Kaleidoscope-LED-ActiveModColor

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-ActiveModColor.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-LED-ActiveModColor

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

With this plugin, any active modifier on the keyboard will have the LED under it
highlighted. No matter how the modifier got activated (a key press, a macro,
anything else), the coloring will apply.

## Using the plugin

To use the plugin, one needs to include the header, and activate the effect. It
is also possible to use a custom color instead of the white default.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LED-ActiveModColor.h>

void setup () {
  USE_PLUGINS(&ActiveModColorEffect);
  
  Kaleidoscope.setup ();
  
  ActiveModColorEffect.highlight_color = CRGB(0x00, 0xff, 0xff);
}
```

It is recommended to place the activation (the `USE_PLUGINS` call) of the plugin
last, so that it can reliably override any other plugins that may work with the
LEDs, and apply the highlight over those.

## Plugin methods

The plugin provides the `ActiveModColorEffect` object, which has the following
property:

### `.highlight_color`

> The color to use for highlighting the modifiers. Defaults to a white color.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
* [Kaleidoscope-Ranges](https://github.com/keyboardio/Kaleidoscope-Ranges)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-LED-ActiveModColor/blob/master/examples/LED-ActiveModColor/LED-ActiveModColor.ino
