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
  LEDDigitalRainEffect.setDropMs(260); // Make the rain fall more slowly
  LEDDigitalRainEffect.setColorChannel(LEDDigitalRainEffect.ColorChannel::BLUE);

  // Optionally switch this LED mode on at startup
  LEDDigitalRainEffect.activate();
}
```

## Plugin methods

The plugin provides the `LEDDigitalRainEffect` object,
which has various public getters and setters for configuration,
as well as the methods associated with all LED mode plugins.

### Configuration

For full documentation and defaults, see the [header file](src/Kaleidoscope-LEDEffect-DigitalRain.h).

- `LEDDigitalRainEffect.getDecayMs()`,
  `LEDDigitalRainEffect.setDecayMs(uint16_t)`:
  get or set the number of milliseconds it takes for a raindrop
  to decay from full intensity
- `LEDDigitalRainEffect.getDropMs()`,
  `LEDDigitalRainEffect.setDropMs(uint16_t)`:
  get or set the number of milliseconds before digital raindrops fall one row
- `LEDDigitalRainEffect.getNewDropProbability()`,
  `LEDDigitalRainEffect.setNewDropProbability(uint8_t)`:
  get or set the *inverse* probability
  of a new raindrop appearing at the top of each column,
  each time drops fall
- `LEDDigitalRainEffect.getTintShadeRatio()`,
  `LEDDigitalRainEffect.setTintShadeRatio(uint8_t)`:
  get or set the intensity level (0 to 255)
  at which pure green should be output;
  this allows the timeshare ratio of tints vs shades of green to be controlled,
  where `0` would mean all tints and `255` would mean all shades
- `LEDDigitalRainEffect.getMaximumTint()`,
  `LEDDigitalRainEffect.setMaximumTint(uint8_t)`:
  get or set the maximum tint of a pixel,
  where `0` would mean nothing brighter than pure green,
  or `255` would mean tint all the way to pure white
- `LEDDigitalRainEffect.getColorChannel()`,
  `LEDDigitalRainEffect.setColorChannel(ColorChannel)`:
  color channel to use, allowing green to be overridden with red or blue

#### Color channels

Color channels are identified with
the `LEDDigitalRainEffect.ColorChannel` enum class.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
