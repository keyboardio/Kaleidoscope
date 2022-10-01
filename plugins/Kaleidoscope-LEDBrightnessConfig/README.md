# LEDBrightnessConfig

The `LEDBrightnessConfig` plugin provides a way to set the brightness of all
LEDs on a keyboard, and persist this setting to EEPROM too.

## Using the plugin

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LEDBrightnessConfig.h>
#include <Kaleidoscope-LEDEffect-Rainbow.h>
#include <Kaleidoscope-FocusSerial.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          LEDControl,
                          LEDOff,
                          LEDRainbowEffect,
                          LEDRainbowWaveEffect,
                          Focus,
                          LEDBrightnessConfig);

void setup() {
  Kaleidoscope.setup();
}
```

## Focus commands

### `led.brightness`

> Without arguments, prints the current brightness.
>
> If an argument is given, it sets the brightness to the desired value, and
> stores it in EEPROM.

## Dependencies

* [Kaleidoscope-EEPROM-Settings](Kaleidoscope-EEPROM-Settings.md)
* [Kaleidoscope-FocusSerial](Kaleidoscope-FocusSerial.md)
