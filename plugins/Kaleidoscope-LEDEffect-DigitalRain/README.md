# Kaleidoscope-LEDEffect-DigitalRain

An LED effect similar to the "digital rain" seen in the "Matrix" films
with green lights flowing downwards on the keyboard.

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
  LEDDigitalRainEffect.setDropMs(260); // Make the rain fall more slowly
  LEDDigitalRainEffect.setColorChannel(LEDDigitalRainEffect.ColorChannel::BLUE);

  // Optionally switch this LED mode on at startup
  LEDDigitalRainEffect.activate();
}
```

## Plugin methods

The plugin provides the `LEDDigitalRainEffect` object, which has various public
getters and setters for configuration, as well as the methods associated with
all LED mode plugins.

### `.getDecayMs()`
### `.setDecayMs(decayMs)`

Gets or sets the number of milliseconds it takes for a raindrop to decay from
full intensity.

Defaults to 2000 milliseconds.

### `.getDropMs()`
### `.setDropMs(dropMs)`

Gets or sets the number of milliseconds before digital raindrops fall one row.

Defaults to 180 milliseconds.

### `.getNewDropProbability()`
### `.setNewDropProbability(probability)`

Get or set the *inverse* probability of a new raindrop appearing at the top of
each column each time drops fall. Must be a value between 0 and 255.

Defaults to 18.

### `.getTintShadeRatio()`
### `.setTintShadeRatio(ratio)`

Get or set the intensity level (between 0 and 255) at which pure green (or red,
or blue) should be output. This allows the timeshare ratio of tints vs shades of
green to be controlled. A tint-shade ratio of 0 means all tints, while 255 means
all shades.

Defaults to 208.

### `.getMaximumTint()`
### `.setMaximumTint(max)`

Gets or sets the maximum tint of a pixel. A value of 0 means nothing brighter
than pure green (or red, or blue), while a value of 255 would mean tinting all
the way to pure white.

Defaults to 192.

### `.getColorChannel()`
### `.setColorChannel(channel)`

Gets or sets the color channel to use. Values can be either of the following:

- `LEDDigitalRainEffect.ColorChannel::RED`
- `LEDDigitalRainEffect.ColorChannel::GREEN`
- `LEDDigitalRainEffect.ColorChannel::BLUE`

Defaults to green.

## Dependencies

* [Kaleidoscope-LEDControl](Kaleidoscope-LEDControl.md)
