# LEDBrightnessControl

The `LEDBrightnessControl` plugin provides a way to change the brightness of all
LEDs on a keyboard.

## Using the plugin

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

### Use with `LEDBrightnessConfig`

`LEDBrightnessControl` does not persist brightness changes.  `LEDBrightnessConfig` will reset the brightness on each restart to the configured default.

## Dependencies

* [Kaleidoscope-LEDControl](Kaleidoscope-LEDControl.md)
