# LED-ActiveLayerKeys

A simple way to light up all keys in the top layer in uniform colors. Unlike
[Colormap](Kaleidoscope-Colormap.md), all keys will be the same color. But this
plugin uses considerably less resources, and is easier to set up as well. A
perfect solution when one wants to quickly see what layer they're on and which
keys are on that layer, with minimal resources and time investment.

## Using the plugin

To use the plugin, one needs to include the header, and activate the effect.
Then, one needs to configure a color map:

```c++
#include "Kaleidoscope.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-LED-ActiveLayerKeys.h"

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          LEDActiveLayerKeysEffect);

void setup () {
  static const cRGB layerColormap[] PROGMEM = {
    CRGB(0xff, 0x00, 0x00),  // red, for the first layer
    CRGB(0x00, 0xff, 0x00),  // green, for the second layer
    CRGB(0x00, 0x00, 0xff),  // blue, for the third layer
  };

  Kaleidoscope.setup();
  // By default, only LEDs for keys on the topmost layer are lit.
  //LEDActiveLayerKeysEffect.lightLowerLayers(false);
  LEDActiveLayerKeysEffect.setColormap(layerColormap);
}
```

## Plugin properties

The plugin provides the `LEDActiveLayerKeysEffect` object, which has the following
methods:

### `.setColormap(colormap)`

> Sets the colormap to the supplied map. Each element of the map should be a
> `cRGB` color, and the map should reside in PROGMEM. The array should have the
> same amount of items as there are layers. Any layer that doesn't have a
> matching entry in the array, will have leds turned off.

### `.lightLowerLayers(boolean)`

> By default, this plugin only lights up LEDs keys on the topmost layer. This
> method allows overriding this default, to have the plugin change the leds of
> all non-blocked keys to the color of their respective layers.

## Dependencies

* [Kaleidoscope-LEDControl](Kaleidoscope-LEDControl.md)
