# LED-ActiveLayerColor

A simple way to light up the keyboard in uniform colors, depending on what layer
one's on. Unlike [Colormap](Colormap.md), all keys will be the same color. But
this plugin uses considerably less resources, and is easier to set up as well. A
perfect solution when one wants to quickly see what layer they're on, with
minimal resources and time investment.

## Using the plugin

To use the plugin, one needs to include the header, and activate the effect.
Then, one needs to configure a color map:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-ActiveLayerColor.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          LEDActiveLayerColorEffect);

void setup () {
  static const cRGB layerColormap[] PROGMEM = {
    CRGB(128, 0, 0),
    CRGB(0, 128, 0)
  };

  Kaleidoscope.setup();
  LEDActiveLayerColorEffect.setColormap(layerColormap);
}
```

## Plugin properties

The plugin provides the `LEDActiveLayerColorEffect` object, which has the following
method:

### `.setColormap(colormap)`

> Sets the colormap to the supplied map. Each element of the map should be a
> `cRGB` color, and the array must have the same amount of items as there are
> layers. The map should reside in PROGMEM.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/LEDs/LED-ActiveLayerColor/LED-ActiveLayerColor.ino
