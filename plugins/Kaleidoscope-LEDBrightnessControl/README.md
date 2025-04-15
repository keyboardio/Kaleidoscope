# LEDBrightnessControl

The `LEDBrightnessControl` plugin provides a way to change the brightness of all
LEDs on a keyboard.

Two new keys can be added to your keymap to make LEDs brighter or dimmer.
Holding Shift reverses the function.

## Using the plugin

Add `Key_LEDBrightnessUp` and/or `Key_LEDBrightnessDown` to your keymap,
and configure as follows.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDBrightnessControl.h>
#include <Kaleidoscope-LEDEffect-Rainbow.h>
#include <Kaleidoscope-FocusSerial.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          LEDControl,
                          LEDOff,
                          LEDRainbowEffect,
                          LEDRainbowWaveEffect,
                          Focus,
                          LEDBrightnessControl);

void setup() {
  Kaleidoscope.setup();
}
```

### Using with `LEDBrightnessConfig`

`LEDBrightnessControl` does not persist brightness changes.
`LEDBrightnessConfig` will reset the brightness on each restart to the
configured default.

## Dependencies

* [Kaleidoscope-LEDControl](Kaleidoscope-LEDControl.md)
