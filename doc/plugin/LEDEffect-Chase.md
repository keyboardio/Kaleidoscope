# Kaleidoscope-LEDEffect-Chase

A simple LED effect where one color chases another across the keyboard and back,
over and over again. Playful colors they are.

## Using the extension

To use the plugin, include the header, and tell the firmware to use it:

```c++
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDEffect-Chase.h>

KALEIDOSCOPE_INIT_PLUGINS(LEDControl,
                          LEDEffect-Chase);

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides the `LEDChaseEffect` object, which has no public methods or
properties, outside of those provided by all LED modes.

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)
