# LEDEffect-Rainbow

Two colorful rainbow effects are implemented by this plugin: one where the
rainbow waves through the keys, and another where the LEDs breathe though the
colors of a rainbow. The difference is that in the first case, we have all the
rainbow colors on display, and it waves through the keyboard. In the second
case, we have only one color at a time, for the whole board, and the color
cycles through the rainbow's palette.

## Using the extension

To use the plugin, include the header, and tell the firmware to use either (or
both!) of the effects:

```c++
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-Rainbow.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDRainbowEffect, LEDRainbowWaveEffect);

void setup() {
  Kaleidoscope.setup();

  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);
  LEDRainbowWaveEffect.update_delay(50);
}
```

## Plugin methods

The plugin provides two objects: `LEDRainbowEffect`, and `LEDRainbowWaveEffect`,
both of which provide the following methods:

### `.brightness([brightness])`

> Sets (or gets, if called without an argument) the LED brightness for the
> effect.
>
> Defaults to 50.

### `.update_delay([delay])`

> Sets (or gets, if called without an argument) the number of milliseconds
> between effect updates. Smaller number results in faster rainbows.
>
> Defaults to 40.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)
