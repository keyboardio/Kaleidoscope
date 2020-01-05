# LEDEffect-Breathe

Provides a breathing effect for the keyboard. Breathe in, breathe out.

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-Breathe.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          LEDBreatheEffect);

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin properties

The plugin provides the `LEDBreatheEffect` object, which has a single property:

### `.hue`

> The hue of the breathe effect.
>
> Defaults to 170, a blue hue.

### `.saturation`

> The color saturation of the breathe effect.
>
> Defaults to 255, the maximum.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)
