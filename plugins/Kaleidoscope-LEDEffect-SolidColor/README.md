# LEDEffect-SolidColor

This plugin provides tools to build LED effects that set the entire keyboard to
a single color. For show, and for backlighting purposes.

## Using the extension

To use the plugin, include the header, declare an effect using the
`kaleidoscope::plugin::LEDSolidColor` class, and tell the firmware to use the
new effect:

```c++
#include <Kaleidoscope-LEDEffect-SolidColor.h>

static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);

KALEIDOSCOPE_INIT_PLUGINS(LEDControl, solidRed);

void setup() {
  Kaleidoscope.setup();
}
```

## Dependencies

* [Kaleidoscope-LEDControl](LEDControl.md)
