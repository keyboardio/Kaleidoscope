# Kaleidoscope-LEDEffect-DigitalRain

An LED effect similar to the "digital rain" seen in the "Matrix" films
with green lights flowing downwards on the keyboard.

![Video of the plugin in action](https://i.imgur.com/LUVFDgT.gif)

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include <Kaleidoscope-LEDEffect-DigitalRain.h>

KALEIDOSCOPE_INIT_PLUGINS(
  LEDDigitalRainEffect
);

void setup() {
  Kaleidoscope.setup();

  // Optionally adjust the configuration
  LEDDigitalRainEffect.DROP_MS = 140; // Make the rain fall faster

  LEDDigitalRainEffect.activate();
}
```

## Plugin methods

The plugin provides the `LEDDigitalRainEffect` object, which has no public methods,
outside of those provided by all LED modes.

## Configuration

A few properties are exposed for configuration.
For their full documentation, see the [header file](src/Kaleidoscope-LEDEffect-DigitalRain.h),
and for the defaults, see the [implementation file](src/Kaleidoscope-LEDEffect-DigitalRain.cpp).

- `LEDDigitalRainEffect.DECAY_MS`:
  number of milliseconds it takes for a raindrop to decay from full intensity
- `LEDDigitalRainEffect.DROP_MS`:
  number of milliseconds before digital raindrops fall one row
- `LEDDigitalRainEffect.NEW_DROP_PROBABILITY`:
  inverse probability of a new raindrop appearing at the top of each column,
  each time drops fall
- `LEDDigitalRainEffect.PURE_GREEN_INTENSITY`:
  this allows the time ratio of tints vs shades of green to be controlled
- `LEDDigitalRainEffect.MAXIMUM_BRIGHTNESS_BOOST`:
  the maximum lightness of a pixel, above pure green
- `LEDDigitalRainEffect.COLOR_CHANNEL`:
  colour channel to use, allowing green to be overridden with red or blue

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
