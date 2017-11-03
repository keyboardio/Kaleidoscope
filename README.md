# Kaleidoscope-LEDEffect-DigitalRain

An LED effect similar to the "digital rain" seen in the "Matrix" films
with green lights flowing downwards on the keyboard.

![Video of the plugin in action](https://i.imgur.com/LUVFDgT.gif)

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include <Kaleidoscope-LEDEffect-DigitalRain.h>

void setup() {
  Kaleidoscope.use(&LEDDigitalRainEffect);

  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides the `LEDDigitalRain` object, which has no public methods or
properties, outside of those provided by all LED modes.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)
